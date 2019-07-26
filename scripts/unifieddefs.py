import os

def readFile(filename):
    with open(filename, 'r') as f:
        return f.readlines()

def writeFile(filename, strlist):
    with open(filename, 'w+') as f:
        f.writelines(strlist)

def replaceLines(lines, dict, sign_left = '/*@', sign_right = '@*/'):

    wholeline = ''
    for line in lines:
        wholeline += line

    for k in dict.keys():
        wholeline = wholeline.replace(sign_left + k + sign_right, dict[k])
    
    newlines_pre = wholeline.split('\n')

    newlines = []
    for x in newlines_pre:
        newlines.append(x + '\n')

    return newlines
    
    

def getDictFromLines(lines): 
    dic = {}
    for x in lines:
        index = x.find('=')
        if index == -1:
            continue
        if x.find('\n', index) == -1:
            dic[x[:index]] = x[index+1:]
        else:
            dic[x[:index]] = x[index+1:-1]
    return dic
    


if __name__ == "__main__":
    lines = readFile('unifieddefs/def1.udf')
    dict = getDictFromLines(lines)
    torreplace = readFile('unifieddefs/test.h')
    newlines = replaceLines(torreplace, dict)
    print(newlines)
    writeFile('unifieddefs/test.hh', newlines)

    filenames_pre = readFile('unifieddefs/deflist.txt')
    filenames = []
    for f in filenames_pre:

        if(f[0] == '#'):
            continue

        if f[-1] == '\n':
            filenames.append(f[0:-1])
        else:
            filenames.append(f)


   
    

        