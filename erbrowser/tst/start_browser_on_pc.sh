MOZILLA_TOP="/home/jorisg/tmp/mozilla"
export LD_LIBRARY_PATH="${MOZILLA_TOP}/dist/lib"
export MOZILLA_FIVE_HOME="${MOZILLA_TOP}/dist/bin/"

#export LD_LIBRARY_PATH="/home/ann/Browsers/mozilla/build_result/dist/Embed/"
#export MOZILLA_FIVE_HOME="/home/ann/Browsers/mozilla/build_result/dist/bin/"
# strace ./browser /home/ann/eReader/examples/wget/www.tijd.be/eink/e20050602/MyNewsPaper.html --sync 2&> straceout.txt
#./browser /home/ann/eReader/examples/wget/www.tijd.be/eink/e20050602/MyNewsPaper.html --sync 

./browser "$1" --sync 

