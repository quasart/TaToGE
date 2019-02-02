#!/bin/bash

VERSION=`grep -w ^VERSION tatoge.pro | grep -o '[^ \s=]*$'`

cp LICENSE release/
cp -r examples/ release/
rm release/examples/all.json

cd release/
zip -r tatoge-$VERSION.zip tatoge.exe examples LICENSE

