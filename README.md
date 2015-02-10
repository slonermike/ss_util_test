# ss_util_test

This project is designed to demonstrate the use of the ss_util library using cmake to generate makefiles.

<h3>After pulling down this project, pull down ss_util</h3>
```
git clone https://github.com/slonermike/ss_util.git ss_util
```

<h3>If you don't have it already, get cmake.</h3>

<b>In OS X</b>
```
brew install cmake
```
<b>In Ubuntu</b>
```
sudo apt-get install cmake
```

<h3>Create the Build Directory and Generate the Makefile</h3>
```
mkdir build
cmake ..
```

<h3>Build The Executable</h3>
<ul>
  <li>If you're not already there, navigate to the 'build' folder</li>
  <li>Run the makefile</li>
</ul>
```
make all
```

<h3>Run the build</h3>
```
./ss_util_test
```
