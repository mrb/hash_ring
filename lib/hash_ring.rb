class HashRing
  VERSION = "0.0.2"

  def print
    self.print_ring
  end

  def to_s
    "<HashRing:#{self.object_id} @num_replicas=#{@num_replicas} @hash_function=#{@hash_function} @nodes=#{@nodes}>"
  end
end

require 'hash_ring/hash_ring'
