puts <<-header
%readonly-tables
%enum
%define lookup-function-name mochilo_encoding_to_id
%define hash-function-name mochilo_encoding_hash
%struct-type
struct mochilo_enc_map { const char *name; uint8_t id; }
%%
header

Encoding.list.each do |encoding|
  puts "#{encoding.name}, MSGPACK_ENC_#{encoding.name.gsub('-', '_').upcase}"
end
