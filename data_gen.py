import random
import sys


# Parameters for functions
surname_length = 6
rest_name_length = 10
address_length = 40

num_records = int(sys.argv[1])
filename = sys.argv[2]

def gen_string(num):
	vowels = ['a','o','e','u','i']

	s = ""

	for x in range(0, num):
		random_vowel = vowels[random.randint(0,4)]		
		s = s + random_vowel

	return s

def gen_name(surname_length, rest_name_length):
	surname = gen_string(surname_length)
	rest_name = gen_string(rest_name_length)

	name = surname + ", " + rest_name	
	
	return name

def gen_address(address_length):
	house_num = random.randint(0,10)

	address = gen_string(address_length)

	if house_num == 0:
		return address
	else:
		s = str(house_num) + ", " + address
		return s

def gen_postcode():
	s = ""
	s = s + gen_string(1).upper()
	s = s + str(random.randint(0,9))
	s = s + str(random.randint(0,9))
	s = s + " "
	s = s + str(random.randint(0,9))
	s = s + gen_string(1).upper()
	s = s + gen_string(1).upper()

	return s

def gen_record():
	s = ""
	s = s + gen_name(surname_length, rest_name_length) + '\n'
	s = s + gen_address(address_length) + '\n'
	s = s + gen_postcode() + '\n'

	return s

output_file = open(filename, "w");

for x in range(0, num_records):
	output_file.write(gen_record())	

output_file.close()
