# Twitter Political Mood Gauge

Before building, you'll need these packages (for debian linux)

###libcurl

* download the appropriate debian file for your architecture from: http://packages.debian.org/sid/libcurl4-gnutls-dev
* then install with this command:
<code>sudo dpkg -i [deb filename]</code>

###libjsoncpp

* download the appropriate debian file for your architecture from: http://packages.debian.org/sid/libjsoncpp-dev
* then install with this command:
<code>sudo dpkg -i [deb filename]</code>

Then to make sure everything is right, just run this command in the root of the download:
<code>make test</code>

You should get a 100% pass rate. If not, make sure you are connected to the internet and that the packages above installed correctly. If all is well, you can then run make to rebuild:
<code>make</code>

And then you are ready to go! Run this script to start:
<code>./run.sh</code>

And open html/index.html in firefox to see the output!
