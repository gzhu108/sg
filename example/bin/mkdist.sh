#!/bin/sh

set -x

SRCDIR="$( cd "$(dirname "$0")/.." ; pwd -P )"
if [ ! -z "$1" ] ; then
    SRCDIR=$1
fi

DISTDIR=dist
if [ ! -z "$2" ] ; then
    DISTDIR=$2
fi

rm -rf $DISTDIR

mkdir -p $DISTDIR/metricator/bin
cp $SRCDIR/metricator/bin/metricator $DISTDIR/metricator/bin
cp -r $SRCDIR/metricator/config/ $DISTDIR/metricator

mkdir -p $DISTDIR/streetgangserver/bin
cp $SRCDIR/streetgangserver/bin/streetgangserver $DISTDIR/streetgangserver/bin
cp -r $SRCDIR/streetgangserver/config/ $DISTDIR/streetgangserver

mkdir -p $DISTDIR/streetgangclient/bin
cp $SRCDIR/streetgangclient/bin/streetgangclient $DISTDIR/streetgangclient/bin
cp -r $SRCDIR/streetgangclient/config/ $DISTDIR/streetgangclient

mkdir -p $DISTDIR/simplewebserver/bin
cp $SRCDIR/simplewebserver/bin/simplewebserver $DISTDIR/simplewebserver/bin
cp -r $SRCDIR/simplewebserver/config/ $DISTDIR/simplewebserver
cp -r $SRCDIR/simplewebserver/webroot/ $DISTDIR/simplewebserver
cp $SRCDIR/simplewebserver/*.key $DISTDIR/simplewebserver
cp $SRCDIR/simplewebserver/*.pem $DISTDIR/simplewebserver

mkdir -p $DISTDIR/bouncer/bin
cp $SRCDIR/bouncer/bin/bouncer $DISTDIR/bouncer/bin
cp -r $SRCDIR/bouncer/config/ $DISTDIR/bouncer

# copy the bash files
cp -r $SRCDIR/bin/ $DISTDIR/bin
