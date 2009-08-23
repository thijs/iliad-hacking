#!/bin/bash

echo ""
echo ""
echo "Work directory = " `pwd`
echo ""
echo "E-reader filesystem = " $EREADER_FILESYSTEM
echo "-------------------"
echo ""


set -e  # stop on error
set -v  # verbose
##set -x  # show executed commands


# Install build environment
autoreconf --install --force
./configure --host=arm-linux --build=i686 --prefix=/usr --datadir=/usr/share

if [ "$EREADER_NO_DIST_CHECK" != "y" ]
then
# Test to see if everything is configured right
# PLEASE LEAVE THIS TEST, IT SAVES A LOT OF TIME WHEN
# BUILDING A RELEASE --- MvdW
DISTCHECK_CONFIGURE_FLAGS="--host=arm-linux --build=i686"
export DISTCHECK_CONFIGURE_FLAGS
make distcheck
#
# remove left-overs from distcheck
tarfile=`grep AC_INIT configure.ac | awk '{
                                              n = split( $0, arr, /[(\[\], )]+/ );
                                              if ( n >= 3 )
                                              {
                                                  package = tolower( arr[2] );
                                                  version = arr[3];
                                                  printf( "%s-%s.tar.gz", package, version );
                                              }
                                          }'`
rm $tarfile
fi

# Build new object files
make clean
make

# Install new object and data files
if [ "$EREADER_FILESYSTEM" == "" ]
then
    echo "--> Set EREADER_FILESYSTEM to install on e-reader nfs filesystem"
else
    make install DESTDIR="$EREADER_FILESYSTEM"
fi

# Done
echo "Done ok"

