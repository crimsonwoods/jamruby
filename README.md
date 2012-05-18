jamruby
====

__jamruby__ is Java to mruby bridge.

Ready to build
----

    $ git clone https://github.com/mruby/mruby.git
    $ git clone https://github.com/crimsonwoods/jamruby.git
    $ cd mruby
    $ git clone https://github.com/crimsonwoods/mruby_ndk-build.git android
    $ make
    $ cd android/jni
    $ ndk-build
    $ cd ../../../jamruby

'mruby' is needed for build 'jamruby'.<br/>
And it has to place on same directory.<br/>
if your environment is not, you have to modify your makefile 'Android.mk'.

How to build the JNI library
----

    $ cd jni
    $ ndk-build

Open project via Eclipse
----

1. import project; select 'Existing Projects into Workspace' on the import wizard.
2. select root directory (cloned directory 'jamruby').
3. build project.
4. 'jamruby' is built as Android library project.

