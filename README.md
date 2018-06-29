# Shuuseki
Build your own corpus for Chinses! It is a small system written by C++ to help you manage your own corpora. With the command line application - **Shuuseki**, you are able to create a Corpus and import the raw materials into it, as well as delete the materials. Well, these are basic features. I have tried to design some interesting functions and you will see them blow.

### Usage
I suppose that the users of **Shuuseki** are familiar to Bash or any other Teminal(eg. cmd in Windows). In other words, the application run based on command line. Next I will demostrate the basic way to use the application.

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
Then, if we wanna know more about the corpus we have built, we can find out the status of the corpus by the command *corpus status*, like this:
```Shell
>>> corpus status
```
Here, we would get the information, like the number of the Kanji(Chinese Characters), and the frequency of the top 20 Kanji.If we were not satisfied about this, we could know more.
```Shell
>>> count -a 2 5 20 30
```
or
```Shell
>>> count -f 2 5 20 30
``` 
With the command *count*, we could gather the infomation of phrase frequency. The result is sorted based on alphabets(option -*a*) or the values of the frequency(option -*f*). The next two options are the range of n-gram, say I set them 2 and 5, which means we want the phrases with 2-5 Kanji. The last two options are the range of the frequency. The list, of course, would be quite long. So we recommand the users try to get part of them, which are regards as the most helpful info.

A corpus should own a function like search, especially complex search. I designed a search pattern for users, for example,
```Shell
>>> search 最 2 5 的
```
The command means the user feels like get the phrases with "最" as its start and "的" as its end. The length of the middle part is in the range 2-5. When completing searching, the application generate a result file in the directory.

That's what the application did. Hope you all like it!
