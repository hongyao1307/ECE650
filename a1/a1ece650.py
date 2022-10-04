import re
import sys
#!/usr/bin/env python3

class Graph:
    def __init__(self):
        self.node_index = 0
        self.nodes = {}
        self.streets = {}
    def cal_intersect(self, streets, new_street_name):
        linesN = get_street_lines(streets, new_street_name)
        for k in self.streets.keys():
            linesN = get_street_lines(streets, new_street_name)
            linesK = get_street_lines(self.streets, k)
            new_cors1, new_cors2 = cal_streets_intersect(self.streets[k], linesK, streets[new_street_name], linesN)
            self.streets[k]= new_cors1
            streets[new_street_name] = new_cors2
        return streets
    def has_street(self, k):
        if k in self.streets.keys():
            return True
        return False
    def get_node_id(self, value):
        is_exist, node_id = self.is_node_exist(value)
        if is_exist:
            return node_id
        return len(self.nodes.keys())+1
    def add_street(self, streets, k):
        s = self.cal_intersect(streets, k)
        self.streets[k] = s[k]
    def generate_graph(self):
        for k in self.streets.keys():
            cors = self.streets[k]
            for i in range(len(cors)-1):
                if len(cors[i])<=2 and len(cors[i+1])<=2:
                    continue
                node_id_1 = self.get_node_id(cors[i])
                self.add_node(node_id_1, cors[i])
                node_id_2 = self.get_node_id(cors[i+1])
                self.add_node(node_id_2, cors[i+1])
    def is_node_exist(self, value):
        for k in self.nodes.keys():
            node = self.nodes[k]
            if node.value == value or \
               (node.value[0],node.value[1],1) == value:
                return True, k
        return False, None
    def add_node(self, node_id, value):
        v = Node(node_id, value)  
        self.nodes[node_id] = v                                                                                                               
        return v                                                                                                                                                                       
    def print_graph(self):
        print("V = {")
        for n in self.nodes.keys():
            self.nodes[n].print_node()
        print('}')
        print("E = {")
        self.print_all_edges()
        print('}')        

    def print_all_edges(self):
        outputs  = []
        for k in self.streets:
            outputs.extend(self.get_street_edges(k))
        if len(outputs)> 0:
            print(",\n".join(outputs))
    def get_street_edges(self, street_name): 
        outputs = []
        cors = self.streets[street_name]
        for i in range(len(cors)-1):
            if len(cors[i])<=2 and len(cors[i+1])<=2:
                continue            
            node_id_1 = self.get_node_id(cors[i])
            node_id_2 = self.get_node_id(cors[i+1])
            output = "<{},{}>".format(node_id_1, node_id_2)
            outputs.append(output)
        return outputs

class Node:                                                                                                                                                                            
    def __init__(self, node_id,value):                                                                                                                                                       
        self.node_id = node_id  
        self.value = value
    def get_node_id(self):
        return self.node_id
    def print_node(self):
        print("{0}: ({1:.2f},{2:.2f})".format(self.node_id, self.value[0], self.value[1]))
    def __str__(self):                                                                                                                                                                 
        return str(self.node_id)

class Point:
    def __init__(self,p):
        self.x=p[0]
        self.y=p[1]

def get_street_lines(streets, name):
    lines = []
    if name not in streets.keys():
        return []
    cor_list = streets[name]
    if len(cor_list)<2:
        return lines
    tmp_cors = []
    for c in cor_list:
        if len(c)==2:
            tmp_cors.append(c)
            
    for i in range(len(tmp_cors)-1):
        line = [tmp_cors[i],tmp_cors[i+1]]
        lines.append(line)    
    return lines

def intersect_insert_before_cor(cors, cor, new_node):
    if new_node in cors:
        return cors
    ref_max = 100
    ref_counter = -1
    for ref_val in range(1, ref_max):
        if (new_node[0], new_node[1], ref_val) in cors:
            cors.remove((new_node[0], new_node[1], ref_val))
            ref_counter = ref_val
            break
    new_cors = []
    i = 0
    index = 0
    for c in cors:
        new_cors.append(c)
        if c == cor:
            index = i
        i = i+1
    if ref_counter < 0:
        new_cors.insert(index, (new_node[0], new_node[1], 1))
    else:
        new_cors.insert(index, (new_node[0], new_node[1], ref_counter+1))
    return new_cors

def cal_streets_intersect(cors1, s1, cors2, s2):
    new_node = None
    new_s1_cors = cors1
    new_s2_cors = cors2
    for l in s1:
        new_node = None
        for m in s2:
            new_node = line_intersection(l, m)
            if new_node:
                if (l[0][0] == new_node[0] and l[0][1]==new_node[1]) or \
                   (l[1][0] == new_node[0] and l[1][1]==new_node[1]):
                    continue
                if m[0][0] == new_node[0] and m[0][1]==new_node[1] or \
                   m[1][0] == new_node[0] and m[1][1]==new_node[1]:
                    continue
                new_s1_cors = intersect_insert_before_cor(new_s1_cors, l[1], new_node)
                new_s2_cors = intersect_insert_before_cor(new_s2_cors, m[1], new_node)
    return new_s1_cors, new_s2_cors
                
def cross(p1,p2,p3):
    x1=p2.x-p1.x
    y1=p2.y-p1.y
    x2=p3.x-p1.x
    y2=p3.y-p1.y
    return x1*y2-x2*y1     

def is_intersec(p1,p2,p3,p4):
    ret = 0
    if(max(p1.x,p2.x)>=min(p3.x,p4.x)
    and max(p3.x,p4.x)>=min(p1.x,p2.x)
    and max(p1.y,p2.y)>=min(p3.y,p4.y)
    and max(p3.y,p4.y)>=min(p1.y,p2.y)):
        
        if(cross(p1,p2,p3)*cross(p1,p2,p4)<=0
           and cross(p3,p4,p1)*cross(p3,p4,p2)<=0):
            ret=1
        else:
            ret=0
    else:
        ret=0
    return ret

def det(a, b):
        return a[0] * b[1] - a[1] * b[0]
    
def line_intersection(line1, line2):
    p1 = Point(line1[0])
    p2 = Point(line1[1])
    p3 = Point(line2[0])
    p4 = Point(line2[1])
    if not is_intersec(p1, p2, p3, p4):
        return None
    xdiff = (line1[0][0] - line1[1][0], line2[0][0] - line2[1][0])
    ydiff = (line1[0][1] - line1[1][1], line2[0][1] - line2[1][1])
    div = det(xdiff, ydiff)
    if div == 0:
        return None
    d = (det(*line1), det(*line2))
    x = det(d, xdiff) / div
    y = det(d, ydiff) / div
    return (x, y)

def add_street(streets, name, coordinates):
    if name.lower() in streets.keys():
        sys.stderr.write("Error: 'add' specified a street that exists.\n")
        return
    streets[name] = coordinates

def mod_street(streets, name, coordinates):
    if name.lower() not in streets.keys():
        sys.stderr.write("Error: 'mod' specified for a street that does not exist.\n")
        return
    streets[name] = coordinates

def rm_street(streets, name):
    if name.lower() not in streets.keys():
        sys.stderr.write("Error: 'rm' specified for a street that does not exist.\n")
        return
    del streets[name]

def gg_graph(streets):
    g = Graph()
    for k in streets.keys():
        if not g.has_street(k):
            g.add_street(streets.copy(), k)
    g.generate_graph()
    g.print_graph()

def main():
    streets = {}
    while True:
        line = sys.stdin.readline().strip()
        if not line:
            break
        command = ""
        index = line.find(" ")
        command = line[:index]
        if index == -1:
            command = line[:]
        i = index
        while line[i] == " ":
            i = i + 1
        if command == 'add' or command == 'mod':
            if index == -1:
                sys.stderr.write(f"Error: '{command}' didn't specify the name of a street.\n")
                continue
            
            if line[i] != '"':
                sys.stderr.write(f"Error: '{command}' didn't specify the name of a street.\n")
                continue
            
            start = i
            i = i + 1
            while i < len(line) and line[i] != '"':
                i = i + 1
            name = line[start:i+1]
            if len(name) < 3 or name[0] != '"' or name[-1] != '"' \
                    or len(re.findall('"', name)) > 2 or (i + 1 < len(line) and line[i+1] != ' '):
                sys.stderr.write(f"Error: '{command}' didn't specify the name of a street or specify a invalid name.\n")
                continue

            if len(re.findall("[^a-z A-Z\"]",name)) > 0:
                sys.stderr.write("Error: characters in the name are alphabetical and space characters only.\n")
            if command=='add' and name in streets.keys():
                sys.stderr.write("Error: street: {} already exists.\n".format(name))
                continue

            if i >= len(line):
                sys.stderr.write("Error: coordinates did not be specified.\n")
                continue

            ss = line[i + 1:].split()
            coordinates = []
            has_error = False
            i = 0
            while i < len(ss):
                tmp = ss[i].split(',')
                if len(tmp) != 2 or len(tmp[0]) < 2 or len(tmp[1]) < 2 \
                        or tmp[0][0] != "(" or tmp[1][-1] != ")":
                    has_error = True
                    break
                else:
                    tmp[0] = tmp[0][1:]
                    tmp[1] = tmp[1][:-1]
                    if tmp[0][0] == '+' or tmp[1][0] == '+':
                        has_error = True
                        break
                    try:
                        coordinates.append((int(tmp[0]), int(tmp[1])))
                    except BaseException:
                        has_error = True
                i = i + 1
            if has_error:
                sys.stderr.write("Error: coordinates has wrong format.\n")
                continue

            if len(coordinates) == 0:
                sys.stderr.write("Error: coordinates did not be specified.\n")
                continue
            
            if len(coordinates) == 1:
                sys.stderr.write("Error: should be at least one pair of coordinates.\n")
                continue
            
            name = name.replace('"',"").lower().strip()
            if len(name)<=0:
                sys.stderr.write("Error: empty Street name.\n")
                continue
            if command == 'add':
                add_street(streets, name, coordinates)
            else:
                mod_street(streets, name, coordinates)

        elif command == 'rm':
            if index == -1:
                sys.stderr.write("Error: 'rm' didn't specify the name of a street.\n")
                continue

            if line[i] != '"':
                sys.stderr.write("Error: 'rm' didn't specify the name of a street.\n")
                continue
            
            start = i
            i = i + 1
            while i < len(line) and line[i] != '"':
                i = i + 1
            name = line[start:i+1]
            if len(name) < 3 or name[0] != '"' or name[-1] != '"' \
                    or len(re.findall('"', name)) > 2 or (i + 1 < len(line) and line[i+1] != ' '):
                sys.stderr.write("Error: 'rm' didn't specify the name of a street or specify a invalid name.\n")
                continue

            if len(re.findall("[^a-z A-Z\"]",name)) > 0:
                sys.stderr.write("Error: characters in the name are alphabetical and space characters only.\n")
            name = name.replace('"',"").lower().strip()
            rm_street(streets, name)

        elif command == 'gg':
            if len(line[2:].strip())>0:
                sys.stderr.write("Error: not a valid command\n")
                continue
            gg_graph(streets)

        else:
            sys.stderr.write(f"Error: '{command}' command does not exist.\n")
            
    sys.exit(0)

if __name__ == '__main__':
    main()
