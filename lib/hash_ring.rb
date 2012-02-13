require 'hash_ring/hash_ring'

class HashRing
  VERSION = "0.0.1"

  attr_accessor :nodes, :num_replicas, :hash_function

  def initialize(num_replicas, hash_function)
    @nodes = []
    @num_replicas = num_replicas
    @hash_function = hash_function
    init(num_replicas, hash_function)
  end

  def add_node(name)
    @nodes << name
    @nodes.uniq!
    self.addNode(name)
  end

  def remove_node(name)
    @nodes.reject!{|n| n == name}
    self.removeNode(name)
  end

  def find_node(name)
    self.findNode(name)
  end

  def print
    self.printRing
  end

  def to_s
    "<HashRing:#{self.object_id} @num_replicas=#{@num_replicas} @hash_function=#{@hash_function} @nodes=#{@nodes}>"
  end
end
