# Twitter Political Mood Gauge

Before building, you'll need these packages (for debian linux)

##libcurl

  * download the appropriate debian file for your architecture from: http://packages.debian.org/sid/libcurl4-gnutls-dev
  * then install with this command:

	sudo dpkg -i [deb filename]


##libjsoncpp

  * download the appropriate debian file for your architecture from: http://packages.debian.org/sid/libjsoncpp-dev
  * then install with this command:

	sudo dpkg -i [deb filename]


Then to make sure everything is right, just run this command in the root of the download:

	make test

You should get a 100% pass rate. If not, make sure you are connected to the internet and that the packages above installed correctly. If all is well, you can then run this command to rebuild:

	make

And then you are ready to go! Run the command below to start:

	./run.sh

And open html/index.html in firefox to see the output!