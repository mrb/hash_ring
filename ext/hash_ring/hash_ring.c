#include "ruby.h"
#include "hash_ring.h"

#define CHECK_RING(data) \
  if (!data) \
    rb_raise(rb_eRuntimeError, "uninitialized data");

VALUE cHashRing;

static VALUE hash_ring_dealloc(void * ring)
{
  if (ring)
    hash_ring_free(ring);
}

static VALUE hash_ring_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, hash_ring_dealloc, NULL);
}

static VALUE method_initialize(VALUE self, VALUE num_replicas, VALUE hash_function)
{
  Check_Type(num_replicas,  T_FIXNUM);
  Check_Type(hash_function, T_FIXNUM);

  hash_ring_t * ring;
  Data_Get_Struct(self, hash_ring_t, ring);

  /* If this method is called twice, free the old ring. */
  if (ring)
    hash_ring_free(ring);

  int nr_int = FIX2INT(num_replicas);
  int hf_int = FIX2INT(hash_function);

  ring = hash_ring_create(nr_int, hf_int);

  VALUE ary = rb_ary_new();

  rb_iv_set(self, "@num_replicas", num_replicas);
  rb_iv_set(self, "@hash_function", hash_function);
  rb_iv_set(self, "@nodes", ary);

  DATA_PTR(self) = ring;

  return self;
}

static VALUE method_add_node(VALUE self, VALUE name)
{
  Check_Type(name, T_STRING);
  hash_ring_t *ring;

  Data_Get_Struct(self, hash_ring_t, ring);
  CHECK_RING(ring);

  char * cname = StringValuePtr(name);

  if(hash_ring_add_node(ring, cname, strlen(cname)) == 0){
    VALUE ary = rb_iv_get(self, "@nodes");
    rb_ary_push(ary, name);
    rb_iv_set(self, "@nodes", ary);
  } else {
    return Qnil;
  }

  return self;
}

static VALUE method_remove_node(VALUE self, VALUE name)
{
  Check_Type(name, T_STRING);

  hash_ring_t * ring;

  char * cname = StringValuePtr(name);

  Data_Get_Struct(self, hash_ring_t, ring);
  CHECK_RING(ring);

  if(hash_ring_remove_node(ring, cname, strlen(cname)) == 0){
    VALUE ary = rb_iv_get(self, "@nodes");
    rb_ary_delete(ary, name);
    rb_iv_set(self, "@nodes", ary);
  } else {
    return Qnil;
  }

  return self;
}

static VALUE method_print_ring(VALUE self)
{
  hash_ring_t * ring;
  Data_Get_Struct(self, hash_ring_t, ring);
  CHECK_RING(ring);

  hash_ring_print(ring);
  return self;
}

static VALUE method_find_node(VALUE self, VALUE key){
  Check_Type(key, T_STRING);

  char * ckey = StringValuePtr(key);

  hash_ring_t * ring;
  Data_Get_Struct(self, hash_ring_t, ring);
  CHECK_RING(ring);

  hash_ring_node_t *node = hash_ring_find_node(ring, ckey, strlen(ckey));

  VALUE cs;

  if(node == NULL){
    rb_raise(rb_eRuntimeError, "No nodes found");
  } else {
    cs = rb_str_new(node->name, node->nameLen);
  }

  return cs;
}

void Init_hash_ring()
{
  cHashRing = rb_define_class("HashRing", rb_cObject);

  rb_define_alloc_func(cHashRing, hash_ring_alloc);

  rb_define_method(cHashRing, "initialize",    method_initialize,   2);
  rb_define_method(cHashRing, "add_node",      method_add_node,     1);
  rb_define_method(cHashRing, "remove_node",   method_remove_node,  1);
  rb_define_method(cHashRing, "find_node",     method_find_node,    1);
  rb_define_method(cHashRing, "print_ring",    method_print_ring,   0);
}
