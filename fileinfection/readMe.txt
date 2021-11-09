
init()

cmdAdd()
	1. Add Node
	2. add Node reference to a map for ID
	3. get Parent Node, and add newNode to parent Node
	4. Return the parent Node file_size
		
cmdMove()
	1. Get Node reference of targetNode;
	2. Erase targetNode from it's parent node -- this parent node filesize also needs to be updated
	3. Get Node reference of pID and add targetNode to this node
	4. return the file_size of the parent node

cmdInfect()
	1. determine increment value;
	2. get targetNode reference from map and infect it
	
cmdRecover()
	1. get node reference of targetNode
	2. recover node

cmdRemove()
	1. get node reference of targetNode
	2. got to node's parent and erase the node

Data (Node)
	clear()
	add()
	refresh()
	erase()
	infect()
	recover()