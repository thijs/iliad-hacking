package=`grep AC_INIT ../configure.ac | awk '{
                                              n = split( $0, arr, /[(\[\], )]+/ );
                                              if ( n >= 3 )
                                              {
                                                  package = tolower( arr[2] );
                                                  version = arr[3];
                                                  printf( "%s", package );
                                              }
                                          }'`

make $package.pot-update
