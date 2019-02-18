#!/bin/sh
rm html/*.html
jw -d vinace-en.dsl#html vinace-en.xml
jw -d vinace-fr.dsl#html vinace-fr.xml

