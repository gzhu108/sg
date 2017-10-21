#!/bin/sh

export DISTDIR=dist

if [ ! -z "$2" ] ; then
    export DISTDIR=$2
fi

rm -rf $DISTDIR
mkdir $DISTDIR

mkdir $DISTDIR/metricator
cp $1 example/metricator/bin/metricator $DISTDIR/metricator
cp $1 -r example/metricator/config/ $DISTDIR/metricator

mkdir $DISTDIR/streetgangserver
cp $1 example/streetgangserver/bin/streetgangserver $DISTDIR/streetgangserver
cp $1 -r example/streetgangserver/config/ $DISTDIR/streetgangserver

mkdir $DISTDIR/streetgangclient
cp $1 example/streetgangclient/bin/streetgangclient $DISTDIR/streetgangclient
cp $1 -r example/streetgangclient/config/ $DISTDIR/streetgangclient

mkdir $DISTDIR/simplewebserver
cp $1 example/simplewebserver/bin/simplewebserver $DISTDIR/simplewebserver
cp $1 -r example/simplewebserver/config/ $DISTDIR/simplewebserver
cp $1 -r example/simplewebserver/webroot/ $DISTDIR/simplewebserver
cp $1 example/simplewebserver/*.key $DISTDIR/simplewebserver
cp $1 example/simplewebserver/*.pem $DISTDIR/simplewebserver

mkdir $DISTDIR/bouncer
cp $1 example/bouncer/bin/bouncer $DISTDIR/bouncer
cp $1 -r example/bouncer/config/ $DISTDIR/bouncer

# copy the bash files
cp $1 -r example/bin/ $DISTDIR/bin
