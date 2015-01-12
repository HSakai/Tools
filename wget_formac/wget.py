# coding: utf-8

import sys, os, urllib2

def dl_progress_str(rate):
	rate_count = rate >> 1
	result = ''
	for i in range(0, rate_count):
		result += '#'
	return result

def main(url, save_path, file_name):
	conn = urllib2.urlopen(url)
	buff_size = 1024 * 100
	content_len = int(conn.headers['content-length'])
	file = open(save_path, 'wb')
	dl_buffs = 0
	message_format = file_name + ' ::[{0}] {1}%'
	while True:
		buffs = conn.read(buff_size)
		if not buffs:
			break
		dl_buffs += len(buffs)
		file.write(buffs)
		dl_rate = int(dl_buffs * 100 / content_len)
		message = message_format.replace('{0}', dl_progress_str(dl_rate)).replace('{1}', str(dl_rate))
		sys.stdout.write("\r%s" % message)
		sys.stdout.flush()
	sys.stdout.write("\n")
	sys.stdout.flush()
	conn.close()
	file.close()

if __name__ == '__main__':
	args = sys.argv
	arg_count = len(args)
	if not (arg_count > 1 and arg_count < 4):
		print('required arguments not enough!!')
		print('first argument is URL')
		print('second argument is save_path (optional arg)')
		sys.exit()

	url = args[1]
	url_delim = url.split('/')
	file_name = url_delim[len(url_delim) - 1]
	save_path = args[2] if arg_count == 3 else './' + file_name
	main(url, save_path, file_name)