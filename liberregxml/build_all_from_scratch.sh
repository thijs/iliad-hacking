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
set -x  # show executed commands


# Install build environment
autoreconf --install --force
./configure --host=arm-linux --build=i686 --prefix=/usr/local/arm/oe/arm-linux --localstatedir=/var --sysconfdir=/etc

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

# Install new libraries and header files
#   Copies libraries and .h files to open embedded on pc (/usr/local/arm/oe/arm-linux)
#   In case of make problems try "sudo make ..." 
for dir in inc src
do
    cd $dir
    make install
    cd ..
done
make install-pkgconfigDATA

# Install new object files
if [ "$EREADER_FILESYSTEM" == "" ]
then
    echo "--> Set EREADER_FILESYSTEM to install on e-reader nfs filesystem"
else
    for dir in src data
    do
        cd $dir
        make install prefix=/usr DESTDIR="$EREADER_FILESYSTEM"
        cd ..
    done
fi

echo "Done ok"

