#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

// Doc: https://tejgop.github.io/nginx-module-guide/

static char *ngx_http_hello_world(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

// ngx_http_hello_world_commands is a static array of directives
static ngx_command_t ngx_http_hello_world_commands[] = {
    // print_hello_world is the directive
    {ngx_string("print_hello_world"),
     // NGX_CONF_NOARGS means no arguments
     NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
     // the mapped function
     ngx_http_hello_world,
     0,
     0,
     NULL},
    ngx_null_command};

/*
ngx_http_hello_world_module_ctx is an array of function references.
The functions will be executed for various purposes such as preconfiguration, postconfiguration, etc.
We don’t need this array in our module, but we still have to define it and fill it with NULLs.
*/
static ngx_http_module_t ngx_http_hello_world_module_ctx = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL};

/*
ngx_http_hello_world_module is an array of definitions for the module.
It tells where the array of directives and functions are (ngx_http_hello_world_module and ngx_http_hello_world_module_ctx).
 We can also add init and exit callback functions. In our module, we don’t need them so we put NULLs instead.
*/
ngx_module_t ngx_http_hello_world_module = {
    NGX_MODULE_V1,
    &ngx_http_hello_world_module_ctx,
    ngx_http_hello_world_commands,
    NGX_HTTP_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING};

/*
ngx_http_hello_world_handler is the heart of our module.
*/
static ngx_int_t ngx_http_hello_world_handler(ngx_http_request_t *r)
{
    // This is what we want to do, print Hello World! on the screen, which should wrap in a HTTP response.
    // We want to print Hello World! on the screen, so we have an unsigned char * with our message in it.
    u_char *ngx_hello_world = (u_char *)"Hello World!";
    // Right after that, there is another variable with the size of the message.
    // note: compile with strlen;
    size_t sz = ngx_strlen(ngx_hello_world);

    // Set the HTTP response header.
    r->headers_out.content_type.len = strlen("text/html") - 1;
    r->headers_out.content_type.data = (u_char *)"text/html";
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = sz;
    ngx_http_send_header(r);

    // Start to set the HTTP response body.
    // ngx_buf_t is a buffer
    ngx_buf_t *b;
    // ngx_chain_t is a chain link. 
    ngx_chain_t *out;

    // ngx_calloc_buf and ngx_alloc_chain_link are Nginx's calloc wrappers
    // that automatically take care of garbage collection.
    b = ngx_calloc_buf(r->pool);
    out = ngx_alloc_chain_link(r->pool);

    // The chain links send responses buffer by buffer and point to the next link.
    out->buf = b;
    // In our module, there is no next link, so we set out->next to Null, which means the response is sent over.
    out->next = NULL;

    // b->pos and b->last help us send our content.
    // b->pos is the first position in the memory.
    b->pos = ngx_hello_world;
    // b->last is the last position.
    b->last = ngx_hello_world + sz;
    // b->memory is set to 1 because our content is read-only. (1 stands for read-only)
    b->memory = 1;
    // b->last_buf tells that our buffer is the last buffer in the request.
    b->last_buf = 1;
    // Now the body was set done.

    // send the body with ngx_http_output_filter.
    return ngx_http_output_filter(r, out);
}

static char *ngx_http_hello_world(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    // We can show Nginx what our
    
    clcf->handler = ngx_http_hello_world_handler;
    return NGX_CONF_OK;
}