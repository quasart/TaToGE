#!/bin/bash

if [[ $# == 0 ]]
then
	echo "usage: $0 <VERSION>"
	exit 1
fi

VERSION=$1

cp LICENSE release/
cp -r examples/ release/

cd release/
zip -r tatoge-$VERSION.zip tatoge.exe examples LICENSE

