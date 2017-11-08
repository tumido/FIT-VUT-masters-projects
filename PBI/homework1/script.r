# author: Tomas Coufal, xcoufa09

# use biocLite
source("https://bioconductor.org/biocLite.R")
biocLite()

# install libraries
biocLite("IRanges")
biocLite("biomaRt")
biocLite("ggplot2")
biocLite("GenomicRanges")

# import
library(IRanges)
library(biomaRt)
library(ggplot2)
library(GenomicRanges)

ensembl <- useMart('ensembl', dataset='hsapiens_gene_ensembl')
attributes <- c(
  'external_gene_name', 'chromosome_name',
  'transcript_start', 'transcript_end',
  'start_position', 'end_position',
  'exon_chrom_start', 'exon_chrom_end'
)

result <- getBM(attributes=attributes, mart=ensembl)
hox <- result[grep('HOX', result$external_gene_name), ]

# Get all IRanges
ir <- IRanges(start=hox$transcript_start, end=hox$transcript_end)

# Sort IRanges by length
ir_by_length <- ir[order(width(ir), decreasing=TRUE), ]

# IRanges without intersectiom
ir_without_intersections <- c(ir_by_length[1])
for (i in 2:length(ir_by_length)) {
  not_intersecting <- TRUE
  for (j in 1:length(ir_without_intersections)) {
    # Check if intersecting
    if (length(intersect(ir_without_intersections[j], ir_by_length[i])) != 0) {
      not_intersecting <- FALSE
      break
    }
  }
  if (not_intersecting) {
    ir_without_intersections <- c(ir_without_intersections, ir_by_length[i])
  }
}

# Init Data Frame
hox_dataframe <- data.frame(matrix(ncol = length(colnames(hox)), nrow = 0))
colnames(hox_dataframe) <- colnames(hox)

# Map IRanges back to genes
for (i in 1:length(ir_without_intersections)) {
  for (j in 1:nrow(hox)) {
    # Find matching gene
    if (start(ir_without_intersections[i]) == hox[j,]$transcript_start & end(ir_without_intersections[i]) == hox[j,]$transcript_end) {
      hox_dataframe <- rbind(hox_dataframe, hox[j,])
      break
    }
  }
}

# Plot the histogram
qplot(width(ir_without_intersections), geom='histogram', bins=10)

# Prepare export table
export_table <- hox_dataframe[c('external_gene_name', 'chromosome_name','transcript_start', 'transcript_end', 'exon_chrom_end', 'exon_chrom_start')]
export_table$exon_count <- 0
export_table$sequence <- 0

for (i in 1:length(hox_dataframe[,1])) {
  export_table$exon_count[i] <- hox_dataframe$exon_chrom_end[i] - hox_dataframe$exon_chrom_start[i]
}


# Find closest cytosin v dinukleotidu CG
for (i in 1:length(hox_dataframe[,1])) {
  # Get sequence
  seq <- getSequence(chromosome=hox_dataframe$chromosome_name[i], start=hox_dataframe$start_position[i], end=hox_dataframe$end_position[i],
                     seqType="5utr", type="entrezgene", mart=ensembl)
  if (length(seq[,1]) > 1) {
    seq <- seq[apply(apply(seq[1], 2, nchar), 2, which.max),]
  }
  if (seq$`5utr` == "Sequence unavailable") {
    next
  }
  export_table$sequence[i] <- seq[1]

  # Find all CGs
  all_cgs <- c(gregexpr("CG", seq)[[1]])

  # Compute closest distance
  seq_indexed <- c()
  for (j in 1:nchar(seq$`5utr`)) {
    seq_indexed <- c(seq_indexed, abs(j - all_cgs[which.min(abs(j - all_cgs))]))
  }

  # Plot and export
  png(filename=paste(c('sequence_', i, '.png'), collapse=''))
  plot(seq_indexed)
  dev.off()
}

# Import data to GRanges
rg <- GRanges(RangedData(ir_without_intersections), name=hox_dataframe$external_gene_name)

# Export Data frame
export_table$sequence <- vapply(export_table$sequence, paste, collapse = ", ", character(1L))
write.csv(export_table, "exported_dataframe.csv")
