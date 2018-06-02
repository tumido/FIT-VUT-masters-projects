#!/bin/sh

JASON_HOME="Jason-1.4.2"

export CLASSPATH=$JASON_HOME/lib/jason.jar:$JASON_HOME/bin/jedit/jedit.jar

java org.gjt.sp.jedit.jEdit
