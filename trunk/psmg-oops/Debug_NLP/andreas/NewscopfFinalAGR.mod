option auxfiles rc;
option presolve 0;

param pi = 4 * atan(1);
param demScaleDemand;
param demScaleFlow;
param freeTap = 0;
param baseMVA;

param RefBus symbolic;
#param   RefBus = "b10" symbolic;

###############################   BUS   ###########################
set     Bus ;
###############################   Parameters   ###########################
# Reference bus:
param   ReferenceFlag{Bus}>=0;
# Demand:
param   DemandP{Bus};
param   DemandQ{Bus};
# Voltage limits:
param   MinVoltage {Bus} >= 0;
param   MaxVoltage {Bus} >= 0;
# shunt conductance and susceptance at buses
param   GS{Bus};
param   BS{Bus};
# PV bus:
param   PVFlag{Bus}>=0;

#set RefBusSet = setof {b in Bus:ReferenceFlag[b]=1} b;

###############################   Generator   ###########################
set     Generator ;
###############################   Parameters   ###########################
# Generator Location
param   Location {Generator} symbolic;
# Generator Limits
param   MinGenP {Generator} >=0;
param   MaxGenP {g in Generator} >= MinGenP[g];
param   MinGenQ {Generator} ;
param   MaxGenQ {g in Generator} >= MinGenQ[g];
# Generator Cost
param   GenConst {Generator} ;
param   GenLin {Generator} ;
param   GenQuad {Generator} ;

set PVBUS within Bus = setof {g in Generator: MaxGenP[g]>0 } Location[g];


###############################   Line   ###########################
set     Line ;
###############################   Parameters   ###########################
# Line:
param   LStartBus {Line} symbolic;
param   LEndBus {Line} symbolic;
# Line Reactance, Resistance
param   LReactance {Line};
param   LResistance {Line} >= 0;
# Line Shunt Capacitance
param   LBsh{l in Line};
# Line Limits
param   LMaxFlow {Line} >= 0;
# MaxFlow in contingency case
param   LContMaxFlow {Line} >= 0;
# N - 1
param   LNminus1 {Line} ;
# Conductance, Susceptance
param   LG {l in Line} := LResistance[l]/(LReactance[l]^2+LResistance[l]^2);
param   LB {l in Line} := -LReactance[l]/(LReactance[l]^2+LResistance[l]^2);


###############################   Transformer   ###########################
set     Transformer ;
###############################   Parameters   ###########################
# Trans Field:
param   TStartBus {Transformer} symbolic;
param   TEndBus {Transformer} symbolic;
# Transformer Reactance, Resistance
param   TReactance {Transformer};
param   TResistance {Transformer} >= 0;
# Transformer Shunt Capacitance
param   TBsh{Transformer};
# Transformer Limits
param   TMaxFlow {Transformer} >= 0;
# TMaxFlow in contingency case
param   TContMaxFlow {Transformer} >= 0;
# Tap ratio
param   MaxTap {Transformer} >= 0;
param   MinTap {Transformer} >= 0;
# N - 1
param   TNminus1 {Transformer};
# TConductance, TSusceptance
param   TG {t in Transformer} := TResistance[t]/(TReactance[t]^2+TResistance[t]^2);
param   TB {t in Transformer} := -TReactance[t]/(TReactance[t]^2+TResistance[t]^2);

param ExTap{Transformer}>=0;



set     LUT = Line union Transformer;
param   StartBus{lu in LUT} := if (lu in Line) then LStartBus[lu] else TStartBus[lu] symbolic;
param   EndBus{lu in LUT} := if (lu in Line) then LEndBus[lu] else TEndBus[lu] symbolic;
param   MaxFlow{lu in LUT} := if (lu in Line) then demScaleFlow*LMaxFlow[lu] else demScaleFlow*TMaxFlow[lu] ;
param   ContMaxFlow{lu in LUT} := if (lu in Line) then demScaleFlow*LContMaxFlow[lu] else demScaleFlow*TContMaxFlow[lu];
param   Bsh{lu in LUT} := if (lu in Line) then LBsh[lu] else TBsh[lu];
param   G{lu in LUT} := if (lu in Line) then LG[lu] else TG[lu];
param   B{lu in LUT} := if (lu in Line) then LB[lu] else TB[lu];


set     Contingencies within LUT = setof {l in Line: LNminus1[l] = 1 } l union setof {t in Transformer: TNminus1[t] = 1 } t;
set     ContPlus = Contingencies union {"opf"} ;


# Problem dimensions
# - Lines l
# - transformers t
# - buses b (of which b0 PVBUS)
# - generators g
# - contingencies c
#
###############################   Variables   ###########################
# variables are
#
# - PpowerGenPV{Generators != RefBus}: first stage                    g-1
# - VoltagePV{PVBUS}: first stage                                     b0     
# - TapRatio{transformers} first stage (only if freeTap==1)           t
#
# - PpowerGenRef{Contingency, GeneratorAtRefBus}                      c+1
# - QPowerGen{Contingencies, Generators}                             (c+1)*g
# - FlowStartP, FlowEndP {Contingency, LUT\{c}}                   2(c+1)(l+t-1)
# - FlowStartQ, FlowEndQ {Contingency, LUT\{c}}                   2(c+1)(l+t-1)
# - Voltages{Contingencies, b\b0}                                  (c+1)*(b-b0)
# - VoltageAngle{Contingencies, Bus\{RefBus}}                       (b-1)*(c+1)
#
# - slackStart/End{LUT}                                               2(l+t-1)
# - contslackStart/End{Contingencies, LUT}                           2c*(l+t-1)
#                                                                  ============
#                                           g-1+b0+t + (c+1)*[6(l+t-1)+g+2b-b0]
# equality variables per contingency: 4*(l+t-1)+g+2b-b0

var PpowerGenPV{ g in Generator: Location[g]!= RefBus and (Location[g] in PVBUS) } >= MinGenP[g], <= MaxGenP[g] ;
var PpowerGenRef{ContPlus, g in Generator: Location[g]= RefBus} >= MinGenP[g], <= MaxGenP[g] ;

#var QpowerGen{ContPlus, g in Generator} >= 1.1*MinGenQ[g], <= 1.1*MaxGenQ[g] ;
var QpowerGen{ContPlus, g in Generator} >= MinGenQ[g], <= MaxGenQ[g] ;

var FlowStartP{ c in ContPlus, l in LUT: l != c} ;
var FlowStartQ{ c in ContPlus, l in LUT: l != c} ;
var FlowEndP{ c in ContPlus, l in LUT: l != c } ;
var FlowEndQ{ c in ContPlus, l in LUT: l != c } ;


#var VoltagePV {b in PVBUS} >= 0.9, <=1.1;  
#var VoltagePV {b in PVBUS} >= 0.94, <=1.06;  
var VoltagePV {b in PVBUS} >= MinVoltage[b], <= MaxVoltage[b];
#var Voltages { c in ContPlus, b in Bus diff PVBUS} >= (if c in Contingencies then 0.8 else 0.90), <= (if c in Contingencies then 1.2 else 1.1);
var Voltages { c in ContPlus, b in Bus diff PVBUS} >= MinVoltage[b], <= MaxVoltage[b];


var VoltageAngle{ ContPlus, Bus diff {RefBus} } ;

var TapRatio{t in Transformer} 
	>= (if freeTap=1 then MinTap[t] else ExTap[t]), 
	<= (if freeTap=1 then MaxTap[t] else ExTap[t]);

#var TapRatio{t in Transformer} >= MinTap[t], <= MaxTap[t];
#param TapRatio{t in Transformer} = ExTap[t];



var contslackStart{ c in Contingencies, l in LUT: l != c} <= ContMaxFlow[l]^2;
var contslackEnd{ c in Contingencies, l in LUT: l != c} <= ContMaxFlow[l]^2;

var slackStart{  l in LUT} <= MaxFlow[l]^2;
var slackEnd{  l in LUT} <=MaxFlow[l]^2;


###############################   Objective   ###########################
minimize Total_Cost:  sum {g in Generator} (GenConst[g]
+
baseMVA*GenLin[g]*(if Location[g]=RefBus then PpowerGenRef["opf",g] else (if Location[g] in PVBUS then PpowerGenPV[g] else 0))
+
baseMVA^2*GenQuad[g]*(if Location[g]=RefBus then PpowerGenRef["opf",g] else (if Location[g] in PVBUS then PpowerGenPV[g] else 0))^2);


###############################   Constraints   ###########################
#
# constraints are
# - KCLP/Q{Contingencies, Bus}                                  2*(c+1)*b
# - P/QKVLStart{Contingencies, LUT\{c}}                         2*(c+1)*(l+t-1)
# - P/QKVLEnd{Contingencies, LUT\{c}}                           2*(c+1)*(l+t-1)
#
# - FlowLimitStart/End{LUT}                                     2*(l+t)
# - contFlowLimitStart/End{Contingencies, LUT}                  2*c*(l+t-1)
#
#

##############  KCL
# KCL + P power
subject to KCLP{c in ContPlus,b in Bus}:
      (sum{g in Generator: Location[g] = b} (if Location[g]=RefBus then PpowerGenRef[c,g] else (if Location[g] in PVBUS then PpowerGenPV[g] else 0)))
      -
      demScaleDemand*DemandP[b]
      =
      (sum{l in LUT: StartBus[l] = b and l!=c} FlowStartP[c,l])
      +
      (sum{l in LUT: EndBus[l] = b and l!=c} FlowEndP[c,l]) 
      +
	GS[b]*(if b in PVBUS then (VoltagePV[b]^2) else (Voltages[c,b]^2));

# KCL + Q power
subject to KCLQ{c in ContPlus,b in Bus}:
    (sum{g in Generator: Location[g] = b}QpowerGen[c,g])
    -
    demScaleDemand*DemandQ[b]
    =
    (sum{l in LUT: StartBus[l]=b and l!=c} FlowStartQ[c,l])
    +
    (sum{l in LUT: EndBus[l]=b and l!=c} FlowEndQ[c,l])
    -
    0.5*(if b in PVBUS then (VoltagePV[b]^2) else (Voltages[c,b]^2))
        *
        (  (sum{l in LUT: StartBus[l]=b and  l!=c} Bsh[l]/(if l in Transformer then (TapRatio[l]^2) else 1))
        +
           (sum{l in LUT: EndBus[l]=b and l!=c}   Bsh[l])  )
      -
	BS[b]*(if b in PVBUS then (VoltagePV[b]^2) else (Voltages[c,b]^2));

##############  KVL  (LINES)
# KVL + P power  at Start bus of lines
subject to PKVLStart{c in ContPlus, l in Line: l!=c}:
        FlowStartP[c,l]=
        (  G[l]*
           (if StartBus[l] in PVBUS then (VoltagePV[StartBus[l]]^2) else (Voltages[c,StartBus[l]]^2) )
           -
           (if StartBus[l] in PVBUS then (VoltagePV[StartBus[l]]) else (Voltages[c,StartBus[l]])  )
           *
           (if EndBus[l] in PVBUS then (VoltagePV[EndBus[l]]) else (Voltages[c,EndBus[l]])  )
           *
           ( G[l]*cos
		(
         	  (if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
        	  -
        	  (if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
		)
             +
             B[l]*sin
		(
        	  (if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
        	  -
        	  (if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
		)
	   )
        );

# KVL + Q power  at Start bus of lines
subject to QKVLStart{c in ContPlus, l in Line: l!=c}:
       FlowStartQ[c,l] =
       (  -B[l]*
          (if StartBus[l] in PVBUS then (VoltagePV[StartBus[l]]^2) else (Voltages[c,StartBus[l]]^2) )
          -
          (if StartBus[l] in PVBUS then (VoltagePV[StartBus[l]]) else (Voltages[c,StartBus[l]]))
          *
          (if EndBus[l] in PVBUS then (VoltagePV[EndBus[l]]) else (Voltages[c,EndBus[l]]))
          *
          ( G[l]*sin
	      (
              	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
        	-
        	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
	      )
       	    -
            B[l]*cos
	      (
        	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
        	-
        	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
	      )
	  )
        );

# KVL + P power  at End bus of lines
subject to PKVLEnd{c in ContPlus, l in Line: l!=c}:
        FlowEndP[c,l]=
        ( G[l]*
          (if EndBus[l] in PVBUS then (VoltagePV[EndBus[l]]^2) else (Voltages[c,EndBus[l]]^2))
          -
          (if StartBus[l] in PVBUS then (VoltagePV[StartBus[l]]) else (Voltages[c,StartBus[l]]))
          *
          (if EndBus[l] in PVBUS then (VoltagePV[EndBus[l]]) else (Voltages[c,EndBus[l]]))
          *
          (  G[l]*cos(
             	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
             	-
             	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
             )
             +
             B[l]*sin(
             	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
             	-
             	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
	     )
	  )
        );

# KVL + Q power  at End bus of lines
subject to QKVLEnd{c in ContPlus, l in Line: l!=c}:
        FlowEndQ[c,l]=
       (  -B[l]*
          (if EndBus[l] in PVBUS then (VoltagePV[EndBus[l]]^2) else (Voltages[c,EndBus[l]]^2) )
          -
          (if StartBus[l] in PVBUS then (VoltagePV[StartBus[l]]) else (Voltages[c,StartBus[l]]))
          *
          (if EndBus[l] in PVBUS then (VoltagePV[EndBus[l]]) else (Voltages[c,EndBus[l]]))
          *
          (  G[l]*sin(
           	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
           	-
           	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
	     )
             -
             B[l]*cos(
             	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
           	-
           	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
	     )
	  )
      );


##############  KVL  (Transformer)
# KVL + P power  at Start bus of Transformer
subject to tPKVLStart{c in ContPlus, l in Transformer: l!=c}:
        FlowStartP[c,l]=
        (  G[l]*
           (if StartBus[l] in PVBUS then (VoltagePV[StartBus[l]]^2) else (Voltages[c,StartBus[l]]^2)) / TapRatio[l]^2
           -
           (if StartBus[l] in PVBUS then (VoltagePV[StartBus[l]]) else (Voltages[c,StartBus[l]])) / TapRatio[l]
           *
           (if EndBus[l] in PVBUS then (VoltagePV[EndBus[l]]) else (Voltages[c,EndBus[l]]))
           *
           (  G[l]*cos(
        	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]]) 
       	 	-
        	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
	      )
              +
              B[l]*sin(
        	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]]) 
        	-
        	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
	      )
	   )
        );

# KVL + Q power  at Start bus of Transformer
subject to tQKVLStart{c in ContPlus, l in Transformer: l!=c}:
       FlowStartQ[c,l] =
       (  -B[l]*
          (if StartBus[l] in PVBUS then (VoltagePV[StartBus[l]]^2) else (Voltages[c,StartBus[l]]^2) ) / TapRatio[l]^2
          -
          (if StartBus[l] in PVBUS then (VoltagePV[StartBus[l]]) else (Voltages[c,StartBus[l]])) / TapRatio[l]
          *
          (if EndBus[l] in PVBUS then (VoltagePV[EndBus[l]]) else (Voltages[c,EndBus[l]]))
          *
          (  G[l]*sin(
        	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
        	-
        	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
	     )
             -
             B[l]*cos(
        	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]]) 
        	-
        	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
 	     )
	  )
       );

# KVL + P power  at End bus of lines, now endbus is Transformer
subject to tPKVLEnd{c in ContPlus, l in Transformer: l!=c}:
        FlowEndP[c,l]=
        (  G[l]*
           (if EndBus[l] in PVBUS then (VoltagePV[EndBus[l]]^2) else (Voltages[c,EndBus[l]]^2) ) 
           -
           (if StartBus[l] in PVBUS then (VoltagePV[StartBus[l]]) else (Voltages[c,StartBus[l]]))
           *
           (if EndBus[l] in PVBUS then (VoltagePV[EndBus[l]]) else (Voltages[c,EndBus[l]])) / TapRatio[l]
           *
           (  G[l]*cos(
             	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
        	-
        	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
	      )
              +
              B[l]*sin(
        	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
        	-
        	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
	      )
	   )
        );

# KVL + Q power  at End bus of lines, now endbus is Transformer
subject to tQKVLEnd{c in ContPlus, l in Transformer: l!=c}:
        FlowEndQ[c,l]=
       (  -B[l]*
          (if EndBus[l] in PVBUS then (VoltagePV[EndBus[l]]^2) else (Voltages[c,EndBus[l]]^2) )
          -
          (if StartBus[l] in PVBUS then (VoltagePV[StartBus[l]]) else (Voltages[c,StartBus[l]]))
          *
          (if EndBus[l] in PVBUS then (VoltagePV[EndBus[l]]) else (Voltages[c,EndBus[l]])) / TapRatio[l]
          *
          (  G[l]*sin(
        	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]]) 
        	-
        	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
	     )
             -
             B[l]*cos(
        	(if EndBus[l]=RefBus then 0 else VoltageAngle[c,EndBus[l]])
        	-
        	(if StartBus[l]=RefBus then 0 else VoltageAngle[c,StartBus[l]])
	     )
	  )
       );









##############  Line and Transformer Thermal Limits
# Flow limit at Start bus of each line
subject to FlowLimitStart{ l in LUT}:
      FlowStartP["opf",l]^2 + FlowStartQ["opf",l]^2 = slackStart[l] ;
# Flow limit at End bus of each line
subject to FlowLimitEnd{ l in LUT}:
      FlowEndP["opf",l]^2 + FlowEndQ["opf",l]^2 = slackEnd[l] ;

# Flow limit at Start bus of each line (Contingency)
subject to conFlowLimitStart{ c in Contingencies, l in LUT: l!=c}:
      FlowStartP[c,l]^2 + FlowStartQ[c,l]^2 = contslackStart[c,l] ;
# Flow limit at End bus of each line  (Contingency)
subject to conFlowLimitEnd{ c in Contingencies, l in LUT: l!=c}:
      FlowEndP[c,l]^2 + FlowEndQ[c,l]^2 = contslackEnd[c,l] ;



#################################### Other constraints which have already been implied by above model

############## Voltage do not change in all PV Bus generators
# subject to BusVolCons{c in Contingencies, b in PVBUS }:
#     Voltages[c,b] = Voltages["opf",b];


############## real power generation do not change in all PV Bus except for RefBus
# subject to BusGenCons{c in Contingencies, g in Generator: Location[g] != RefBus }:
#     PpowerGen[c,g] = PpowerGen["opf",g];

##############  Reference Bus Phase     has already been implied by above model
#subject to RefAngle{c in ContPlus}:
#      VoltageAngle[c, RefBus] = 0;



