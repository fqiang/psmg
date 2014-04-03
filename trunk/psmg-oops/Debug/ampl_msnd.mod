set NODES ordered;
set ARCS ordered;
set COMM ordered;

param cost{ARCS};
param basecap{ARCS};
param arc_source{ARCS} symbolic;
param arc_target{ARCS} symbolic;
param comm_source{COMM} symbolic;
param comm_target{COMM} symbolic;
param comm_demand{COMM};

param b{k in COMM, i in NODES} :=
	if(comm_source[k]==i) then comm_demand[k] else
		if(comm_target[k]==i) then -comm_demand[k] else 0;

var sparecap{ARCS}>=0;

block MCNFArcs{a in ARCS}: {
	set ARCSDIFF = ARCS diff {a};
	
	block ANet{k in COMM}: {
		var Flow{ARCSDIFF}>=0;
		subject to FlowBalance{i in NODES}:
			sum{j in ARCSDIFF:arc_target[j]==i}( Flow[j] )- sum{x in ARCSDIFF:arc_source[x]==i} (Flow[x]) = b[k,i];
	}
	var capslack{ARCSDIFF} >= 0;
	subject to Capacity{j in ARCSDIFF}:
		sum{k in COMM} (ANet[k].Flow[j]) + capslack[j] - sparecap[j] = basecap[j];
}

block MCNFNodes{n in NODES}: {
	set NODESDIFF = NODES diff {n};
	set ARCSDIFF = {m in ARCS:arc_source[m]!=n && arc_target[m]!=n};
	
	block NNet{k in COMM}: {
		var Flow{ARCSDIFF} >= 0;
		subject to FlowBlance{i in NODESDIFF}:
			sum{j in ARCSDIFF:arc_target[j]==i}(Flow[j]) - sum{j in ARCSDIFF:arc_source[j]==i}(Flow[j]) = b[k,i];
	}

	var capslacknode{ARCSDIFF} >= 0;
	subject to Capacity{j in ARCSDIFF}:
		sum{k in COMM} (NNet[k].Flow[j]) + capslacknode[j] - sparecap[j] = basecap[j];
}

minimize costToInstall: sum{a in ARCS} (sparecap[a]*cost[a]);
	
