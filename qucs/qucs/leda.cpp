

- Simulation engines: Qucs, NgSpice, Xyce
- Components: R, L and C
- Sources: AC/DC voltage/current
- Probes: voltage/current
- Simulation type: AC, DC and Transient + Parameter sweep
- diagram: Cartesian (w/ specialisisation? amplitude/phase bode plots, ...)
- no equations

Symbols:
 - R: US and european
 - C: neutral and polar

Components:
===========

R;ngspice;R{{ID}} {{PORT0_NET}} {{PORT1_NET}}  {{R}}
R;qucsator;R:{{ID}} {{PORT0_NET}} {{PORT1_NET}} R="{{R}}" Temp="{{Temp}}" Tc1="{{Tc1}}" Tc2="{{Tc2}}" Tnom="{{Tnom}}"
R;xyce;R{{ID}} {{PORT0_NET}} {{PORT1_NET}}  {{R}}

<architecture id="ngspice" entityId="resistor">
 <template>
R{{ID}} {{PORT0_NET}} {{PORT1_NET}} {{R}} 
 </template>
</architecture>
<architecture id="qucsator" entityId="resistor">
 <template>
R:{{ID}} {{PORT0_NET}} {{PORT1_NET}} R="{{R}}" Temp="{{Temp}}" Tc1="{{Tc1}}" Tc2="{{Tc2}}" Tnom="{{Tnom}}"
 </template>
</architecture>
<architecture id="xyce" entityId="resistor">
 <template>
R{{ID}} {{PORT0_NET}} {{PORT1_NET}} {{R}} 
 </template>
</architecture>

C;ngspice;C{{ID}} {{PORT0_NET}} {{PORT1_NET}}  {{C}}  IC={{V}}
C;qucsator;C:{{ID}} {{PORT0_NET}} {{PORT1_NET}} C="{{C}}" V="{{V}}"
C;xyce;C{{ID}} {{PORT0_NET}} {{PORT1_NET}}  {{C}}  IC={{V}}

<architecture id="ngspice" entityId="capacitor">
 <template>
C{{ID}} {{PORT0_NET}} {{PORT1_NET}} {{C}} IC={{V}}
 </template>
</architecture>
<architecture id="qucsator" entityId="capacitor">
 <template>
C:{{ID}} {{PORT0_NET}} {{PORT1_NET}} C="{{C}}" V="{{V}}"
 </template>
</architecture>
<architecture id="xyce" entityId="capacitor">
 <template>
C{{ID}} {{PORT0_NET}} {{PORT1_NET}} {{C}} IC={{V}}
 </template>
</architecture>

L;ngspice;L{{ID}} {{PORT0_NET}} {{PORT1_NET}}  {{L}}  IC={{I}}
L;qucsator;L:{{ID}} {{PORT0_NET}} {{PORT1_NET}} L="{{L}}" I="{{I}}"
L;xyce;L{{ID}} {{PORT0_NET}} {{PORT1_NET}}  {{L}}  IC={{I}}

<architecture id="ngspice" entityId="inductor">
 <template>
L{{ID}} {{PORT0_NET}} {{PORT1_NET}} {{L}} IC={{I}}
 </template>
</architecture>
<architecture id="qucsator" entityId="inductor">
 <template>
L:{{ID}} {{PORT0_NET}} {{PORT1_NET}} L="{{L}}" I="{{I}}"
 </template>
</architecture>
<architecture id="xyce" entityId="inductor">
 <template>
L{{ID}} {{PORT0_NET}} {{PORT1_NET}} {{L}} IC={{I}}
 </template>
</architecture>

Sources:
========
Vdc;ngspice;V{{ID}} {{PORT0_NET}} {{PORT1_NET}} DC {{U}}
Vdc;qucsator;Vdc:{{ID}} {{PORT0_NET}} {{PORT1_NET}} U="{{U}}"
Vdc;xyce;V{{ID}} {{PORT0_NET}} {{PORT1_NET}} DC {{U}}

Idc;ngspice;I{{ID}} {{PORT0_NET}} {{PORT1_NET}} DC {{I}}
Idc;qucsator;Idc:{{ID}} {{PORT0_NET}} {{PORT1_NET}} I="{{I}}"
Idc;xyce;I{{ID}} {{PORT0_NET}} {{PORT1_NET}} DC {{I}}

Vac;ngspice;V{{ID}} {{PORT0_NET}} {{PORT1_NET}} DC 0 SIN(0 {{U}} {{F}} 0 {{Theta}}) AC {{U}}
Vac;qucsator;Vac:{{ID}} {{PORT0_NET}} {{PORT1_NET}} U="{{U}}" f="{{f}}" Phase="{{Phase}}" Theta="{{Theta}}"
Vac;xyce;V{{ID}} {{PORT0_NET}} {{PORT1_NET}} DC 0 SIN(0 {{U}} {{F}} 0 {{Theta}}) AC {{U}}

Iac;ngspice;I{{ID}} {{PORT0_NET}} {{PORT1_NET}} DC 0 SIN(0 {{I}} {{F}} 0 {{Theta}}) AC {{I}}
Iac;qucsator;Iac:{{ID}} {{PORT0_NET}} {{PORT1_NET}} I="{{I}}" f="{{f}}" Phase="{{Phase}}" Theta="{{Theta}}"
Iac;xyce;I{{ID}} {{PORT0_NET}} {{PORT1_NET}} DC 0 SIN(0 {{I}} {{F}} 0 {{Theta}}) AC {{I}}

Probes:
=======

IProbe;ngspice;V{{ID}} {{PORT0_NET}} {{PORT1_NET}} DC 0 AC 0
IProbe;qucsator;IProbe:{{ID}} {{PORT0_NET}} {{PORT1_NET}}
IProbe;xyce;V{{ID}} {{PORT0_NET}} {{PORT1_NET}} DC 0 AC 0

VProbe;ngspice;E{{ID}} {{ID}} 0 {{PORT0_NET}} {{PORT1_NET}} 1.0
VProbe;qucsator;VProbe:{{ID}} {{PORT0_NET}} {{PORT1_NET}}
VProbe;xyce;E{{ID}} {{ID}} 0 {{PORT0_NET}} {{PORT1_NET}} 1.0


Simulations:
============

Sweep:
 - instance id + generic id
 - type: linear, logarithmic (10 or e), value list (const/none ?)
 - range (start/nbSteps/stop)

<sweep instanceId="" genericId="">
 <!-- range or list -->
 <range scale="linear|decade|octave">
  <start></start>
  <stop></stop>
  <steps></steps>
 </range> 
 <list>0.1,3.6,5.2,9.9</list>
</sweep>

AC: frequency sweep on a source

.AC;ngspice;AC LIN {{Points}} {{START}} {{STOP}}
.AC;qucsator;.AC:{{ID}} Type="{{Type}}" Start="{{Start}}" Stop="{{Stop}}" Points="{{Points}}" Noise="{{Noise}}"
.AC;xyce;.AC LIN {{Points}} {{START}} {{STOP}}


.DC;ngspice;
.DC;qucsator;.DC:{{ID}} Temp="{{Temp}}" reltol="{{reltol}}" abstol="{{abstol}}" vntol="{{vntol}}" saveOPs="{{saveOPs}}" MaxIter="{{MaxIter}}" saveAll="{{saveAll}}" convHelper="{{convHelper}}" Solver="{{Solver}}"
.DC;xyce;

.TR;ngspice;TRAN nan 0 0
.TR;qucsator;.TR:{{ID}} Type="{{Type}}" Start="{{Start}}" Stop="{{Stop}}" Points="{{Points}}" IntegrationMethod="{{IntegrationMethod}}" Order="{{Order}}" InitialStep="{{InitialStep}}" MinStep="{{MinStep}}" MaxIter="{{MaxIter}}" reltol="{{reltol}}" abstol="{{abstol}}" vntol="{{vntol}}" Temp="{{Temp}}" LTEreltol="{{LTEreltol}}" LTEabstol="{{LTEabstol}}" LTEfactor="{{LTEfactor}}" Solver="{{Solver}}" relaxTSR="{{relaxTSR}}" initialDC="{{initialDC}}" MaxStep="{{MaxStep}}"
.TR;xyce;.TRAN nan 0 0
