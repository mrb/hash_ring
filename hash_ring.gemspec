Gem::Specification.new do |s|
  s.name        = 'hash_ring'
  s.version     = '0.0.1'
  s.date        = '2012-02-12'
  s.summary     = "A wrapper for a fast C hash ring library"
  s.description = "Wraps https://github.com/mikeyk/hash-ring"
  s.authors     = ["Michael R. Bernstein"]
  s.email       = 'michaelrbernstein@gmail.com'
  s.files = Dir.glob('lib/**/*.rb') +
            Dir.glob('ext/**/*.{c,h,rb}')
  s.extensions = ['ext/hash_ring/extconf.rb']

  s.homepage    = 'http://github.com/mrb/hash_ring'
end
