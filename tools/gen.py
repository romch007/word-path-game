import sys

dict_file = sys.argv[1]
words_len = sys.argv[2]
output_file = sys.argv[3]

if dict_file is None or words_len is None:
    print("Bad arguments")


words_len = int(words_len)

file = open(dict_file, "r", encoding='utf-8')
raw = file.read()
form = raw.split("\n")
form2 = list(filter(lambda s: len(s) == words_len, form))

output_file = open(output_file, "w", encoding='utf-8')
for s in form2:
    output_file.write(f"{s}\n")

