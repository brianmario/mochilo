# Loaded by script/console. Land helpers here.

Pry.config.prompt = lambda do |context, nesting, pry|
  "[mochilo] #{context}> "
end
