jamruby
================================

__jamruby__ is Java to mruby bridge.

# Ready to build

    $ git clone https://github.com/mruby/mruby.git
    $ git clone https://github.com/crimsonwoods/jamruby.git

'mruby' is needed for build 'jamruby'.
And it has to place on same directory.
if your environment is not, you have to modify your makefile 'Android.mk'.

# How to build JNI library

    $ cd jni
    $ ndk-build

# Open project via Eclipse

1. import project; select 'Existing Projects into Workspace' on the import wizard.
2. select root directory (cloned directory 'jamruby').
3. build project.
4. 'jamruby' is built as Android library project.
