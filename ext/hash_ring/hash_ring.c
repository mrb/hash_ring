#include "ruby.h"
#include "hash_ring.h"

VALUE cHashRing;

static VALUE hash_ring_dealloc(void * ring)
{
  hash_ring_free(ring);
}

static VALUE hash_ring_alloc(VALUE klass)
{
  hash_ring_t * ring;
  VALUE obj = Data_Make_Struct(klass, hash_ring_t, NULL, hash_ring_dealloc, ring);
  return obj;
}

static VALUE method_initialize(VALUE self, VALUE num_replicas, VALUE hash_function)
{
  Check_Type(num_replicas,  T_FIXNUM);
  Check_Type(hash_function, T_FIXNUM);

  hash_ring_t * ring;
  hash_ring_t * new_ring;

  int nr_int = FIX2INT(num_replicas);
  int hf_int = FIX2INT(hash_function);

  Data_Get_Struct(self, hash_ring_t, ring);

  new_ring = hash_ring_create(nr_int, hf_int);

  MEMCPY(ring, new_ring, hash_ring_t, 1);

  hash_ring_free(new_ring);

  return self;
}

static VALUE method_add_node(VALUE self, VALUE name)
{
  Check_Type(name, T_STRING);
  hash_ring_t *ring;

  Data_Get_Struct(self, hash_ring_t, ring);

  char * cname = StringValuePtr(name);
  hash_ring_add_node(ring, cname, strlen(cname));

  return self;
}

static VALUE method_remove_node(VALUE self, VALUE name)
{
  Check_Type(name, T_STRING);

  hash_ring_t * ring;

  char * cname = StringValuePtr(name);

  Data_Get_Struct(self, hash_ring_t, ring);

  hash_ring_remove_node(ring, cname, strlen(cname));

  return self;
}

static VALUE method_print_ring(VALUE self)
{
  hash_ring_t * ring;
  Data_Get_Struct(self, hash_ring_t, ring);

  hash_ring_print(ring);
  return self;
}

static VALUE method_find_node(VALUE self, VALUE key){
  Check_Type(key, T_STRING);

  char * ckey = StringValuePtr(key);

  hash_ring_t * ring;
  Data_Get_Struct(self, hash_ring_t, ring);

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

  rb_define_method(cHashRing, "init",         method_initialize,   2);
  rb_define_method(cHashRing, "addNode",      method_add_node,     1);
  rb_define_method(cHashRing, "removeNode",   method_remove_node,  1);
  rb_define_method(cHashRing, "findNode",     method_find_node,    1);
  rb_define_method(cHashRing, "printRing",    method_print_ring,   0);
}
