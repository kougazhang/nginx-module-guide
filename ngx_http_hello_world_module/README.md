## How to build the module

From here download source: [nginx source code](http://nginx.org/download)

**In the nginx source**, run configure, make, and make install. If you only want to build the modules and not the Nginx server itself, you can run make modules.

`--prefix` is the relative path. The blow commands should be run in the dictionary nginx source.

```
$ ./configure \
> --prefix=/where/i/want/to/install/nginx \
> --add-dynamic-module=/path/to/ngx_http_hello_world_module

# Build module and Nginx

$ make
$ make install

# Build module

$ make modules

# the cp step is no longer needed
$ cp objs/ngx_http_hello_world_module.so <nginx_install_location>/modules
```

## Using the module

To use the module, edit your nginx.conf file found in the conf directory in the install location.

When you’re done, you can run nginx (<nginx_install_location>/sbin/nginx) and take a look at your work at localhost:8000/test. You should get a blank page saying Hello World!. If so, congratulations! You made your first Nginx module! This module is the base for making any handler.

## Launching nginx

```
./bin/sbin/nginx -p ./bin -c ../ngx_http_hello_world_module/nginx.conf
```

Test the module:

```
curl http://127.0.0.1:8000/test
```

The expected output: Hello World!