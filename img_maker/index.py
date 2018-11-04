""" Convert arg[1] image file to display-block-bytes array """
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

DISPLAY_WIDTH = 64
LINE_HEIGHT = 8

def parse_pixels(pixels, width, pos_x, page):
  """ Convert 8 pixels (up-down) to display-block """
  num = 0
  for pos_y in range(0, LINE_HEIGHT):
    pix = pixels[(page*LINE_HEIGHT+pos_y)*width+pos_x][0]
    if pix >= 0x80:
      num |= 1 << pos_y
  return num

def main(args):
  """ Start point """

  if len(args) != 2:
    print("Invalid count of arguments")
    return 1

  from PIL import Image
  img = Image.open(args[1])
  pixels = img.getdata()
  width, height = img.size
  pages = []
  for page in range(0, int(height/LINE_HEIGHT)):
    out_bytes = []
    for pos_x in range(0, width):
      num = parse_pixels(pixels, width, pos_x, page)
      out_bytes.append(hex(num))
    pages.append("\n\t{" + ",".join(out_bytes) + "}")
  print("{" + ",".join(pages) + "\n}")

  return 0

if __name__ == '__main__':
  import sys
  sys.exit(main(sys.argv))
