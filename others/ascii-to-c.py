#input: https://www.ascii-art-generator.org/, HTML WITH TABLE.

with open('aaaa.html') as f:
    cont = f.read().replace('&#160;</tt>', ' </font></tt>')

def ccode(c):
    colors = ['000000', '0000aa', '00aa00', '00aaaa', 'aa0000', 'aa00aa', 'aa5500', 'aaaaaa', '555555', '5555ff', '55ff55', '55ffff', 'ff5555', 'ff55ff', 'ffff55', 'ffffff']
    code = colors.index(c)
    return code
    
def gen_code(bcolor, fcolor, char):
    flag = ccode(fcolor) + 16 * (ccode(bcolor) % 8)
    #print('{}, {}, '.format(ord(char), flag))
    print(bcolor, fcolor,char)

prev_bcolor = None
for line in cont.split('\n'):
    count = 0
    print('len=', len(line.split('</font>')))
    for pixel in line.split('</font>'):
        pos1 = pixel.find('bgcolor')
        pos2 = pixel.find('font color')
        if pos1 == -1 and pos2 == -1:
            continue
        if pos1 == -1:
            bcolor = prev_bcolor
        else:
            bcolor = pixel[pos1+10:][:6]
            prev_bcolor = bcolor

        if pos2 == -1:
            # Only space may has no font
            gen_code(bcolor, bcolor, ' ')
            count+= 1
        else:
            fcolor = pixel[pos2+13:][:6]
            pos3 = pos2 + 21
            pos4 = pixel.find('<', pos3)
            char = pixel[pos3:pos4] if pos4 != -1 else pixel[pos3:]
            for c in char:
                gen_code(bcolor, fcolor, c)
                count+= 1
    print(count)
    #print('=================')
        

