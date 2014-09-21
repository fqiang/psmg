param baseMVA;
param demScaleDemand;
param demScaleFlow;
param RefBus symbolic;

#BUS
set Bus;
param DemandP{Bus};
param DemandQ{Bus};
param MinVoltage{Bus} >=0;
param MaxVoltage{Bus} >=0;
param GS{Bus};
param BS{Bus};

#Line
set Line;
param LStartBus{Line} symbolic;
param LEndBus{Line} symbolic;
param LReactance{Line};
param LResistance{Line};
param LBsh{Line};
param LMaxFlow{Line} >=0;
param LContMaxFlow{Line} >=0;
param LNminus1{Line};
param LG{l in Line} := LResistance[l]/(LReactance[l]^2 + LResistance[l]^2);
param LB{l in Line} := -LReactance[l]/(LReactance[l]^2 + LResistance[l]^2);

#Transformer
set Transformer;
param TStartBus{Transformer} symbolic;
param TEndBus{Transformer} symbolic;
param TReactance{Transformer};
param TResistance{Transformer};
param TBsh{Transformer};
param TMaxFlow{Transformer} >=0;
param TContMaxFlow{Transformer} >=0;
param TNminus1{Transformer};
param TB{l in Transformer} := TResistance[l]/(TReactance[l]^2 + TResistance[l]^2);
param TG{l in Transformer} :=-TReactance[l]/(TReactance[l]^2 + TResistance[l]^2);
param ExTap{Transformer} >=0;

# Generator
set Generator;
param Location{Generator} symbolic;
param MinGenP{Generator} >= 0;
param MaxGenP{g in Generator} >= MinGenP[g];
param MinGenQ{Generator};
param MaxGenQ{g in Generator} >= MinGenQ[g];
param GenConst{Generator};
param GenLin{Generator};
param GenQuad{Generator};



#computed parameters
set LUT = Line union Transformer;
param StartBus{lu in LUT}:= if (lu in Line) then LStartBus[lu] else TStartBus[lu] symbolic;
param EndBus{lu in LUT}:= if (lu in Line) then LEndBus[lu] else TEndBus[lu] symbolic;
param MaxFlow{lu in LUT}:= if(lu in Line) then demScaleFlow*LMaxFlow[lu] else demScaleFlow*TMaxFlow[lu];
param ContMaxFlow{lu in LUT}:= if(lu in Line) then demScaleFlow*LContMaxFlow[lu] else demScaleFlow*TContMaxFlow[lu];
param Bsh{lu in LUT} := if (lu in Line) then LBsh[lu] else TBsh[lu];
param G{lu in LUT}  := if (lu in Line) then LG[lu] else TG[lu];
param B{lu in LUT} := if (lu in Line) then LB[lu] else TB[lu];
set PVBUS within Bus = setof {g in Generator: MaxGenP[g]>0} Location[g];
set PQBUS = Bus diff PVBUS;
param TapRatio{t in Transformer} := ExTap[t]; 
set Contingencies within LUT = setof {r in Line: LNminus1[r] == 1} r union setof{t in Transformer: TNminus1[t] == 1} t;



################################################################
#root stage
#
###############################################################

#root level variables
var Voltages{b in Bus} >= MinVoltage[b], <= MaxVoltage[b];
var PpowerGen{g in Generator } >= MinGenP[g], <= MaxGenP[g];
var QpowerGen{g in Generator} >= MinGenQ[g], <= MaxGenQ[g];
var FlowStartP{l in LUT};
var FlowStartQ{l in LUT};
var FlowEndP{l in LUT};
var FlowEndQ{l in LUT};
var VoltageAngles{Bus};

var slackStart{l in LUT} <= MaxFlow[l]^2;
var slackEnd{l in LUT} <= MaxFlow[l]^2;

#var Voltages_slack(b in Bus} >=0;
#subject to Voltages_LB{b in Bus}:
#	Voltages[b] - MinVoltage[b] >=0;
#subject to Voltages_UB{b in Bus}:
#	-Voltages[b] + MaxGenP[g] >=0;



#KCL + P power
subject to KCLP{b in Bus}:
	sum{g in Generator: Location[g] == b} (PpowerGen[g])
	- sum{l in LUT: StartBus[l] == b} ( FlowStartP[l])
	- sum{l in LUT: EndBus[l] == b} (FlowEndQ[l])
	- GS[b]*(Voltages[b]^2)
	= 
	demScaleDemand*DemandP[b] 
	;

#KCL + Q power
subject to KCLQ{b in Bus}:
	sum{g in Generator: Location[g] == b} (QpowerGen[g])
	- sum{l in LUT: StartBus[l] == b } (FlowStartQ[l])
	- sum{l in LUT: EndBus[l] == b } (FlowEndQ[l])
	+ 0.5*( Voltages[b]^2)
		*
		(	sum{l in Line: LStartBus[l] == b } (LBsh[l]) 
		+   sum{t in Transformer: TStartBus[t] == b} (TBsh[t]/TapRatio[t]^2)
		+   sum{l in LUT: EndBus[l] == b} (Bsh[l]) 
		)
	+ BS[b]*(Voltages[b]^2)
	=
	demScaleDemand*DemandQ[b]
	;

#KVL +P Power at start bus of lines
subject to PKVLStart{l in Line}:
	FlowStartP[l] 
	- G[l]*(Voltages[StartBus[l]]^2)
	+ Voltages[StartBus[l]] * Voltages[EndBus[l]]
		*
		( 
		  G[l] * cos(VoltageAngles[StartBus[l]] - VoltageAngles[EndBus[l]])
		  +
		  B[l] * sin(VoltageAngles[StartBus[l]] - VoltageAngles[EndBus[l]])
		) 
	= 
	0 	
	;

#KVL +Q power at Start bus of lines
subject to QKVLStart{l in Line}:
	FlowStartQ[l] 
	+ B[l]*(Voltages[StartBus[l]]^2) 
	+
	Voltages[StartBus[l]]*Voltages[EndBus[l]]
	*
	(
	  G[l] * sin(VoltageAngles[StartBus[l]] - VoltageAngles[EndBus[l]]) 
	  -
	  B[l] * cos(VoltageAngles[StartBus[l]] - VoltageAngles[EndBus[l]])
	)
	=
	0
	;

#kVL +P power at End bus of lines
subject to PKVLEnd{l in Line}:
	FlowEndP[l]
	- G[l]*(Voltages[EndBus[l]]^2)
	+ Voltages[StartBus[l]] * Voltages[EndBus[l]]
	*
	(
		G[l]* cos(VoltageAngles[EndBus[l]] - VoltageAngles[StartBus[l]] )
		+
		B[l]* sin(VoltageAngles[EndBus[l]] - VoltageAngles[StartBus[l]] )
	)
	=
	0	
	;
#KVL +Q power at End bus of lines
subject to QKVLEnd{l in Line}:
	FlowEndQ[l]
	+ B[l]*(Voltages[EndBus[l]]^2)
	+ Voltages[StartBus[l]] * Voltages[EndBus[l]]
	*
	(
		G[l]* sin(VoltageAngles[EndBus[l]] - VoltageAngles[StartBus[l]] )
		-
		B[l]* cos(VoltageAngles[EndBus[l]] - VoltageAngles[StartBus[l]] )
	)
	=
	0
	;

################# KVL Transformer
#KVL +P power at start bus of transformer
subject to tPKVLStart{l in Transformer}:
	FlowStartP[l] 
	- G[l]*(Voltages[StartBus[l]]^2)/(TapRatio[l]^2)
	+ Voltages[StartBus[l]]*Voltages[EndBus[l]]/TapRatio[l]
	*
	(
		G[l]*cos(VoltageAngles[StartBus[l]] - VoltageAngles[EndBus[l]] )
		+
		B[l]*sin(VoltageAngles[StartBus[l]] - VoltageAngles[EndBus[l]] )
	)
	=
	0
	;
#KVL +P power at end bus of transformer
subject to tPKVLEnd{l in Transformer}:
	FlowEndP[l] 
	-G[l]*Voltages[EndBus[l]]^2 
	+ Voltages[StartBus[l]]*Voltages[EndBus[l]]/TapRatio[l]
	*
	(	
		G[l]* cos(VoltageAngles[EndBus[l]] - VoltageAngles[StartBus[l]]   )
		+
		B[l]* sin(VoltageAngles[EndBus[l]] - VoltageAngles[StartBus[l]]  )
	)
	=
	0
	;

#KVL +Q power at start bus of transformer
subject to tQKVLStart{l in Transformer}:
	FlowStartQ[l]  	
	+ B[l]*(Voltages[StartBus[l]]^2)/(TapRatio[l]^2)
	+ Voltages[StartBus[l]]/TapRatio[l]
	*
	(
		G[l]*sin(VoltageAngles[StartBus[l]] - VoltageAngles[EndBus[l]])
		-
		B[l]*cos(VoltageAngles[StartBus[l]] - VoltageAngles[EndBus[l]])
	)
	=
	0	
	;
#KVL +Q power at end bus of transformer
subject to tQKVLEnd{l in Transformer}:
	FlowEndQ[l] 
	+ B[l]*(Voltages[EndBus[l]]^2 )
	+ Voltages[StartBus[l]]*Voltages[EndBus[l]]/TapRatio[l]
	*
	(
		G[l]*sin(VoltageAngles[EndBus[l]] - VoltageAngles[StartBus[l]])
		-
		B[l]*cos(VoltageAngles[EndBus[l]] - VoltageAngles[StartBus[l]])
	)
	=
	0
	;

#reference bus 
subject to RefBusZero:
	VoltageAngles[RefBus] = 0;


#############Line and Transformer Thermal Limits
#Flow limit at Start bus of each line
subject to FlowLimitStart{l in LUT}:
	FlowStartP[l]^2 + FlowStartQ[l]^2 - slackStart[l] = 0;
subject to FlowLimitEnd{l in LUT}:
	FlowEndP[l]^2 + FlowEndQ[l]^2 - slackEnd[l] = 0;

######################################################################
# 2nd Stage 
# Contingency cases 
######################################################################
block Contingency{c in Contingencies}: {
	set LUTDIFF = LUT diff {c};
	set LineDIFF= Line diff {c};
	set TransformerDIFF = Transformer diff {c};

	var cVoltages{b in Bus} >= MinVoltage[b], <= MaxVoltage[b];
	var cPpowerGen{g in Generator } >= MinGenP[g], <= MaxGenP[g];
	var cQpowerGen{g in Generator} >= MinGenQ[g], <= MaxGenQ[g];
	var cFlowStartP{l in LUTDIFF};
	var cFlowStartQ{l in LUTDIFF};
	var cFlowEndP{l in LUTDIFF};
	var cFlowEndQ{l in LUTDIFF};
	var cVoltageAngles{Bus};
	
	var cslackStart{l in LUTDIFF} <= MaxFlow[l]^2;
	var cslackEnd{l in LUTDIFF} <= MaxFlow[l]^2;

	
	#KCL + P power
	subject to KCLP{b in Bus}:
		sum{g in Generator: Location[g] == b} (cPpowerGen[g]) 
		- sum{l in LUTDIFF: StartBus[l] == b} (cFlowStartP[l])
		- sum{l in LUTDIFF: EndBus[l] == b} (cFlowEndQ[l])
		- GS[b]*(cVoltages[b]^2)
		=
		demScaleDemand*DemandP[b] 
		;
		

	#KCL + Q power
	subject to KCLQ{b in Bus}:
		sum{g in Generator: Location[g] == b} (cQpowerGen[g]) 
		- sum{l in LUTDIFF: StartBus[l] == b } (cFlowStartQ[l])
		- sum{l in LUTDIFF: EndBus[l] == b } (cFlowEndQ[l])
		+
		0.5*( cVoltages[b]^2)
		*
		(	sum{l in LineDIFF: LStartBus[l] == b } (LBsh[l]) 
		+	sum{t in TransformerDIFF: TStartBus[t] == b} (TBsh[t]/TapRatio[t]^2)
		+ 	sum{l in LUTDIFF: EndBus[l] == b} (Bsh[l]) 
		)
		+ BS[b]*(cVoltages[b]^2)
		=
		demScaleDemand*DemandQ[b]

		;

	#KVL +P Power at start bus of lines
	subject to PKVLStart{l in LineDIFF}:
		cFlowStartP[l]
		- G[l]*(cVoltages[StartBus[l]]^2)
		+ cVoltages[StartBus[l]] * cVoltages[EndBus[l]]
		*
		( 
		  G[l] * cos(cVoltageAngles[StartBus[l]] - cVoltageAngles[EndBus[l]])
		  +
		  B[l] * sin(cVoltageAngles[StartBus[l]] - cVoltageAngles[EndBus[l]])
		) 
		=
		0;

	#KVL +Q power at Start bus of lines
	subject to QKVLStart{l in LineDIFF}:
		cFlowStartQ[l] 
		+ B[l]*(cVoltages[StartBus[l]]^2) 
		+ cVoltages[StartBus[l]]*cVoltages[EndBus[l]]
		*
		(
		  G[l] * sin(cVoltageAngles[StartBus[l]] - cVoltageAngles[EndBus[l]]) 
		  -
		  B[l] * cos(cVoltageAngles[StartBus[l]] - cVoltageAngles[EndBus[l]])
		)
		=
		0
		;

	#kVL +P power at End bus of lines
	subject to PKVLEnd{l in LineDIFF}:
		cFlowEndP[l] 
		- G[l]*(cVoltages[EndBus[l]]^2)
		+ cVoltages[StartBus[l]] * cVoltages[EndBus[l]]
		*
		(
			G[l]* cos(cVoltageAngles[EndBus[l]] - cVoltageAngles[StartBus[l]] )
			+
			B[l]* sin(cVoltageAngles[EndBus[l]] - cVoltageAngles[StartBus[l]] )
		)
		=
		0
		;
	#KVL +Q power at End bus of lines
	subject to QKVLEnd{l in LineDIFF}:
		cFlowEndQ[l] 
		+ B[l]*(cVoltages[EndBus[l]]^2)
		+ cVoltages[StartBus[l]] * cVoltages[EndBus[l]]
		*
		(
			G[l]* sin(cVoltageAngles[EndBus[l]] - cVoltageAngles[StartBus[l]] )
			-
			B[l]* cos(cVoltageAngles[EndBus[l]] - cVoltageAngles[StartBus[l]] )
		)
		=
		0
		;

	################# KVL Transformer
	#KVL +P power at start bus of transformer
	subject to tPKVLStart{l in TransformerDIFF}:
		cFlowStartP[l] 
		-G[l]*(cVoltages[StartBus[l]]^2)/(TapRatio[l]^2)
		+
		(Voltages[StartBus[l]]*Voltages[EndBus[l]]/TapRatio[l])
		*
		(
			G[l]*cos(cVoltageAngles[StartBus[l]] - cVoltageAngles[EndBus[l]] )
			+
			B[l]*sin(cVoltageAngles[StartBus[l]] - cVoltageAngles[EndBus[l]] )
		)
		=
		0
		;
	#KVL +P power at end bus of transformer
	subject to tPKVLEnd{l in TransformerDIFF}:
		cFlowEndP[l] 		
		-G[l]*cVoltages[EndBus[l]]^2 
		+ 
		cVoltages[StartBus[l]]*cVoltages[EndBus[l]]/TapRatio[l]
		*
		(	
			G[l]* cos(cVoltageAngles[EndBus[l]] - cVoltageAngles[StartBus[l]]   )
			+
			B[l]* sin(cVoltageAngles[EndBus[l]] - cVoltageAngles[StartBus[l]]   )
		)
		=
		0
		;

	#KVL +Q power at start bus of transformer
	subject to tQKVLStart{l in TransformerDIFF}:
		cFlowStartQ[l] 
		+ B[l]*(cVoltages[StartBus[l]]^2)/(TapRatio[l]^2)
		+ cVoltages[StartBus[l]]/TapRatio[l]
		*
		(
			G[l]*sin(cVoltageAngles[StartBus[l]] - cVoltageAngles[EndBus[l]])
			-
			B[l]*cos(cVoltageAngles[StartBus[l]] - cVoltageAngles[EndBus[l]])
		)
		=
		0
		;
	#KVL +Q power at end bus of transformer
	subject to tQKVLEnd{l in TransformerDIFF}:
		cFlowEndQ[l]
		+ B[l]*(cVoltages[EndBus[l]]^2 )
		+ cVoltages[StartBus[l]]*cVoltages[EndBus[l]]/TapRatio[l]
		*
		(
			G[l]*sin(cVoltageAngles[EndBus[l]] - cVoltageAngles[StartBus[l]])
			-
			B[l]*cos(cVoltageAngles[EndBus[l]] - cVoltageAngles[StartBus[l]])
		)
		=
		0
		;

	#reference bus 
	subject to RefBusZero:
		cVoltageAngles[RefBus] = 0;


	#############Line and Transformer Thermal Limits
	#Flow limit at Start bus of each line
	subject to FlowLimitStart{l in LUTDIFF}:
		cFlowStartP[l]^2 + cFlowStartQ[l]^2 - cslackStart[l] = 0;
	subject to FlowLimitEnd{l in LUTDIFF}:
		cFlowEndP[l]^2 + cFlowEndQ[l]^2 - cslackEnd[l] = 0;

	##########################################
	#Linking constraints to first stage
	##########################################
	subject to BusVolCons{b in PVBUS}: cVoltages[b] - Voltages[b] = 0;
	#noteA: duplicate temporary set created for {g in Generator: Location[g]!=RefBus and (Location[g] in PVBUS)}	
	subject to BusGenCons{g in Generator: Location[g]!=RefBus and (Location[g] in PVBUS) }: cPpowerGen[g] - PpowerGen[g] = 0;
}

#noteA: this can be used to replace constraint BusGenCons/BusVolCons above, 
#however PSMG not yet support two dimensional constraints index yet!
#subject to BusGenCons{g in Generator:Location[g]!=RefBus and (Location[g] in PVBUS), c in Contingencies}: 
#	Contingency[c].cPpowerGen[g] = PpowerGen[g];

#subject to BusVolCons{b in PVBUS, c in Contingencies}:
#	Contingency[c].cVoltages[b] = Voltages[b];

minimize Total_Cost: sum{ g in Generator} (
		GenConst[g] 
		+ baseMVA*GenLin[g]*PpowerGen[g]
		+ (baseMVA^2)*GenQuad[g]*(PpowerGen[g]^2)
		);
