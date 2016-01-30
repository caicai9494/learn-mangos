This folder contains sample code from [Bartosz Milewski](https://www.youtube.com/watch?v=80ifzK3b8QQ&index=1&list=PL1835A90FC78FF8BE)'s youtube video tutorial series.

In order to compile, 
concurrency[1-5].cc requires 
```
-std=c++11 -pthread
```

concurrency[6-9].cc requires 
```
-std=c++11 -pthread -lboost_system -lboost_filesystem
```
