# sine-generator
generate sinewaves with Bash and C

### coproc

Execution time benefits realized by using Bash's [coproc](http://wiki.bash-hackers.org/syntax/keywords/coproc) feature:

No coproc (thousands of individual `bc -l` invocations):

```
$ time ./sine.sh 4096 83
[...]
real    0m36.183s
user    0m17.686s
sys     0m19.087s
```

Coproc (one `bc -l` coproc):

```
$ time ./sine.sh 4096 83
[...]
real    0m10.725s
user    0m6.589s
sys     0m4.628s
```
