# Shuuseki
Build your own corpus for Chinses! It is a small system written by C++ to help you manage your own corpora. With the command line application - **Shuuseki**, you are able to create a Corpus and import the raw materials into it, as well as delete the materials. Well, these are basic features. I have tried to design some interesting functions and you will see them blow.

### Usage
I suppose that the users of Shuuseki are familiar to Bash or any other Teminal(eg. cmd in Windows). In other words, the application run based on command line. Next I will demostrate the basic way to use the application.

You could run the application by double-click the executive file, exec file[Shuuseki] on Mac or Shuuseki.exe on Windows. Also, you are able to run it just like shell script. So step one, open the termianl and access to the directory where the executive file **Shuuseki** located.

```Shell
$ cd Shuuseki/cmake-build-debug && ./Shuuseki
```
We will see the basic introduction of the application. In the introduction, the commands to manage the corpora are listed here. First, we could create a Corpus by using the command *create*.

```Shell
>>> create Jumoku[the name of the corpus]
```

If it was created successfully, we could import some row meterials into it. We are allowed to import one or more files.
```Shell
>>> import example.txt
```
or
```Shell
>>> import example1.txt example2.txt example3.txt example4.txt
```

We can import somthing, certainly we can delete them.
```Shell
>>> delete example.txt
```
or
```Shell
>>> delete example1.txt example2.txt example3.txt example4.txt
```



