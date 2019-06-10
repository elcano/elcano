<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.4">
<drawing>
<settings>
<setting alwaysvectorfont="yes"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="9" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="con-subd">
<description>&lt;b&gt;SUB-D Connectors&lt;/b&gt;&lt;p&gt;
Harting&lt;br&gt;
NorComp&lt;br&gt;
&lt;p&gt;
PREFIX :&lt;br&gt;
H = High density&lt;br&gt;
F = Female&lt;br&gt;
M = Male&lt;p&gt;
NUMBER: Number of pins&lt;p&gt;
SUFFIX :&lt;br&gt;
H = Horizontal&lt;br&gt;
V = Vertical &lt;br&gt;
P = Shield pin on housing&lt;br&gt;
B = No mounting holes&lt;br&gt;
S = Pins individually placeable (Single)&lt;br&gt;
D = Direct mounting &lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="F25D">
<description>&lt;b&gt;SUB-D&lt;/b&gt;</description>
<wire x1="-19.177" y1="6.9342" x2="-18.669" y2="7.4422" width="0.1524" layer="21" curve="-90"/>
<wire x1="18.542" y1="7.4422" x2="19.05" y2="6.9342" width="0.1524" layer="21" curve="-90"/>
<wire x1="-19.177" y1="1.8542" x2="-19.177" y2="6.9342" width="0.1524" layer="21"/>
<wire x1="-19.812" y1="1.2192" x2="-19.177" y2="1.8542" width="0.1524" layer="21" curve="90"/>
<wire x1="19.05" y1="1.8542" x2="19.05" y2="6.9342" width="0.1524" layer="21"/>
<wire x1="19.05" y1="1.8542" x2="19.685" y2="1.2192" width="0.1524" layer="21" curve="90"/>
<wire x1="-18.669" y1="7.4422" x2="18.542" y2="7.4422" width="0.1524" layer="21"/>
<wire x1="-18.6436" y1="0.508" x2="-18.6436" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-18.6436" y1="0.508" x2="-18.5166" y2="0.508" width="0.1524" layer="21"/>
<wire x1="-18.5166" y1="-1.905" x2="-17.8816" y2="-2.54" width="0.1524" layer="21" curve="90"/>
<wire x1="-18.5166" y1="-1.905" x2="-18.5166" y2="0.508" width="0.1524" layer="21"/>
<wire x1="-17.901" y1="-2.54" x2="17.901" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="18.6436" y1="0.508" x2="18.6436" y2="1.016" width="0.1524" layer="21"/>
<wire x1="18.5166" y1="-1.905" x2="18.5166" y2="0.508" width="0.1524" layer="21"/>
<wire x1="18.5166" y1="0.508" x2="18.6436" y2="0.508" width="0.1524" layer="21"/>
<wire x1="17.8816" y1="-2.54" x2="18.5166" y2="-1.905" width="0.1524" layer="21" curve="90"/>
<wire x1="-24.6888" y1="-2.7432" x2="-23.3172" y2="-4.1148" width="0" layer="48"/>
<wire x1="-23.3172" y1="-4.1148" x2="-21.9456" y2="-2.7432" width="0" layer="48"/>
<wire x1="-21.9456" y1="-2.7432" x2="-20.574" y2="-4.1148" width="0" layer="48"/>
<wire x1="-20.574" y1="-4.1148" x2="-19.2024" y2="-2.7432" width="0" layer="48"/>
<wire x1="19.2024" y1="-2.7432" x2="20.574" y2="-4.1148" width="0" layer="48"/>
<wire x1="20.574" y1="-4.1148" x2="21.9456" y2="-2.7432" width="0" layer="48"/>
<wire x1="21.9456" y1="-2.7432" x2="23.3172" y2="-4.1148" width="0" layer="48"/>
<wire x1="23.3172" y1="-4.1148" x2="24.6888" y2="-2.7432" width="0" layer="48"/>
<smd name="1" x="-16.4594" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="2" x="-13.716" y="-6.045" dx="1.7" dy="3.5" layer="1"/>
<smd name="3" x="-10.9726" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="4" x="-8.2298" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="5" x="-5.4862" y="-6.0446" dx="1.7" dy="3.5" layer="1"/>
<smd name="6" x="-2.7434" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="7" x="0" y="-6.045" dx="1.7" dy="3.5" layer="1"/>
<smd name="8" x="2.7434" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="14" x="-15.0876" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="9" x="5.4862" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="10" x="8.2298" y="-6.0446" dx="1.7" dy="3.5" layer="1"/>
<smd name="11" x="10.9726" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="12" x="13.716" y="-6.045" dx="1.7" dy="3.5" layer="1"/>
<smd name="13" x="16.4594" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="15" x="-12.3444" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="16" x="-9.6012" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="17" x="-6.858" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="18" x="-4.1148" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="19" x="-1.3716" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="20" x="1.3716" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="21" x="4.1148" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="22" x="6.858" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="23" x="9.6012" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="24" x="12.3444" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="25" x="15.0876" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<text x="-11.6332" y="-10.16" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.4732" y="-10.16" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-16.8928" y="-2.1786" size="1.27" layer="21" ratio="10">1</text>
<text x="19.6596" y="-2.54" size="1.27" layer="48">Board</text>
<text x="-17.2212" y="4.5974" size="1.27" layer="51" ratio="10">F25D</text>
<text x="15.5578" y="-2.0786" size="1.27" layer="21" ratio="10">13</text>
<text x="15.9146" y="-0.825" size="1.27" layer="22" ratio="10" rot="MR0">25</text>
<text x="-14.0496" y="-0.825" size="1.27" layer="22" ratio="10" rot="MR0">14</text>
<text x="-24.7396" y="-2.54" size="1.27" layer="48">Board </text>
<rectangle x1="-17.0592" y1="-6.56" x2="-15.8592" y2="-4.06" layer="51"/>
<rectangle x1="-15.6876" y1="-6.56" x2="-14.4876" y2="-4.06" layer="52"/>
<rectangle x1="-12.9444" y1="-6.56" x2="-11.7444" y2="-4.06" layer="52"/>
<rectangle x1="-10.2012" y1="-6.56" x2="-9.0012" y2="-4.06" layer="52"/>
<rectangle x1="-14.316" y1="-6.56" x2="-13.116" y2="-4.06" layer="51"/>
<rectangle x1="-11.5728" y1="-6.56" x2="-10.3728" y2="-4.06" layer="51"/>
<rectangle x1="-8.8296" y1="-6.56" x2="-7.6296" y2="-4.06" layer="51"/>
<rectangle x1="-6.0864" y1="-6.56" x2="-4.8864" y2="-4.06" layer="51"/>
<rectangle x1="-7.458" y1="-6.56" x2="-6.258" y2="-4.06" layer="52"/>
<rectangle x1="-4.7148" y1="-6.56" x2="-3.5148" y2="-4.06" layer="52"/>
<rectangle x1="-1.9716" y1="-6.56" x2="-0.7716" y2="-4.06" layer="52"/>
<rectangle x1="0.7716" y1="-6.56" x2="1.9716" y2="-4.06" layer="52"/>
<rectangle x1="-3.3432" y1="-6.56" x2="-2.1432" y2="-4.06" layer="51"/>
<rectangle x1="-0.6" y1="-6.56" x2="0.6" y2="-4.06" layer="51"/>
<rectangle x1="2.1432" y1="-6.56" x2="3.3432" y2="-4.06" layer="51"/>
<rectangle x1="-26.6" y1="0.7" x2="26.6" y2="1.3" layer="21"/>
<rectangle x1="-17.4" y1="-4.1" x2="17.4" y2="-2.5" layer="21"/>
<rectangle x1="3.5148" y1="-6.56" x2="4.7148" y2="-4.06" layer="52"/>
<rectangle x1="4.8864" y1="-6.56" x2="6.0864" y2="-4.06" layer="51"/>
<rectangle x1="7.6296" y1="-6.56" x2="8.8296" y2="-4.06" layer="51"/>
<rectangle x1="6.258" y1="-6.56" x2="7.458" y2="-4.06" layer="52"/>
<rectangle x1="9.0012" y1="-6.56" x2="10.2012" y2="-4.06" layer="52"/>
<rectangle x1="11.7444" y1="-6.56" x2="12.9444" y2="-4.06" layer="52"/>
<rectangle x1="10.3728" y1="-6.56" x2="11.5728" y2="-4.06" layer="51"/>
<rectangle x1="13.116" y1="-6.56" x2="14.316" y2="-4.06" layer="51"/>
<rectangle x1="14.4876" y1="-6.56" x2="15.6876" y2="-4.06" layer="52"/>
<rectangle x1="15.8592" y1="-6.56" x2="17.0592" y2="-4.06" layer="51"/>
</package>
<package name="F25H">
<description>&lt;b&gt;SUB-D&lt;/b&gt;</description>
<wire x1="-18.669" y1="17.907" x2="-18.415" y2="17.907" width="0.1524" layer="21"/>
<wire x1="-19.177" y1="17.399" x2="-18.669" y2="17.907" width="0.1524" layer="21" curve="-90"/>
<wire x1="18.669" y1="17.907" x2="19.177" y2="17.399" width="0.1524" layer="21" curve="-90"/>
<wire x1="-18.796" y1="11.684" x2="18.796" y2="11.684" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="7.62" x2="-21.463" y2="7.62" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="7.62" x2="-26.543" y2="10.668" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="7.62" x2="-26.543" y2="7.493" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="10.668" x2="-26.543" y2="11.176" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="11.176" x2="-26.543" y2="11.684" width="0.1524" layer="21"/>
<wire x1="26.543" y1="11.684" x2="26.543" y2="11.176" width="0.1524" layer="21"/>
<wire x1="26.543" y1="11.176" x2="26.543" y2="10.668" width="0.1524" layer="21"/>
<wire x1="26.543" y1="10.668" x2="26.543" y2="7.62" width="0.1524" layer="21"/>
<wire x1="26.543" y1="7.62" x2="26.543" y2="7.493" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="11.176" x2="-24.003" y2="11.176" width="0.1524" layer="21"/>
<wire x1="-24.003" y1="11.176" x2="-21.463" y2="11.176" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="10.414" x2="-21.463" y2="10.668" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="10.668" x2="-21.463" y2="11.176" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="10.668" x2="-21.336" y2="10.668" width="0.1524" layer="21"/>
<wire x1="-21.336" y1="8.255" x2="-20.701" y2="7.62" width="0.1524" layer="21" curve="90"/>
<wire x1="-21.463" y1="7.62" x2="-20.574" y2="7.62" width="0.1524" layer="21"/>
<wire x1="-20.574" y1="7.62" x2="-20.32" y2="7.62" width="0.1524" layer="21"/>
<wire x1="-21.336" y1="8.255" x2="-21.336" y2="10.668" width="0.1524" layer="21"/>
<wire x1="-20.32" y1="7.62" x2="-20.32" y2="6.858" width="0.1524" layer="21"/>
<wire x1="-20.32" y1="7.62" x2="20.32" y2="7.62" width="0.1524" layer="21"/>
<wire x1="-20.32" y1="6.858" x2="20.32" y2="6.858" width="0.1524" layer="21"/>
<wire x1="20.32" y1="7.62" x2="20.32" y2="6.858" width="0.1524" layer="21"/>
<wire x1="20.32" y1="7.62" x2="20.574" y2="7.62" width="0.1524" layer="21"/>
<wire x1="20.574" y1="7.62" x2="21.463" y2="7.62" width="0.1524" layer="21"/>
<wire x1="24.003" y1="11.176" x2="26.543" y2="11.176" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="11.176" x2="21.463" y2="11.176" width="0.1524" layer="21"/>
<wire x1="21.463" y1="11.176" x2="24.003" y2="11.176" width="0.1524" layer="21"/>
<wire x1="21.463" y1="10.414" x2="21.463" y2="10.668" width="0.1524" layer="21"/>
<wire x1="21.463" y1="10.668" x2="21.463" y2="11.176" width="0.1524" layer="21"/>
<wire x1="21.336" y1="8.255" x2="21.336" y2="10.668" width="0.1524" layer="21"/>
<wire x1="21.336" y1="10.668" x2="21.463" y2="10.668" width="0.1524" layer="21"/>
<wire x1="20.701" y1="7.62" x2="21.336" y2="8.255" width="0.1524" layer="21" curve="90"/>
<wire x1="-25.908" y1="-3.175" x2="-26.543" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-2.54" x2="-26.543" y2="7.493" width="0.1524" layer="21"/>
<wire x1="-21.082" y1="-3.175" x2="-25.908" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-21.082" y1="-3.175" x2="-20.574" y2="-2.667" width="0.1524" layer="21"/>
<wire x1="-20.574" y1="-2.667" x2="-20.574" y2="7.62" width="0.1524" layer="21"/>
<wire x1="20.574" y1="-2.667" x2="20.574" y2="7.62" width="0.1524" layer="21"/>
<wire x1="20.574" y1="-2.667" x2="21.082" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="21.082" y1="-3.175" x2="26.035" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="26.035" y1="-3.175" x2="26.543" y2="-2.667" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-2.667" x2="26.543" y2="7.493" width="0.1524" layer="21"/>
<wire x1="-2.7686" y1="-1.143" x2="-2.7686" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="-8.2804" y1="-1.143" x2="-8.2804" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="-11.049" y1="-1.143" x2="-11.049" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="-9.652" y1="1.397" x2="-9.652" y2="2.413" width="0.8128" layer="51"/>
<wire x1="-6.9088" y1="1.397" x2="-6.9088" y2="2.413" width="0.8128" layer="51"/>
<wire x1="-1.3716" y1="1.397" x2="-1.3716" y2="2.413" width="0.8128" layer="51"/>
<wire x1="2.7686" y1="-1.143" x2="2.7686" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="6.8834" y1="1.397" x2="6.8834" y2="2.413" width="0.8128" layer="51"/>
<wire x1="8.2804" y1="-1.143" x2="8.2804" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="4.1402" y1="1.397" x2="4.1402" y2="2.413" width="0.8128" layer="51"/>
<wire x1="5.5118" y1="-1.143" x2="5.5118" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="1.3716" y1="1.397" x2="1.3716" y2="2.413" width="0.8128" layer="51"/>
<wire x1="0" y1="-1.143" x2="0" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="-4.1402" y1="1.397" x2="-4.1402" y2="2.413" width="0.8128" layer="51"/>
<wire x1="-5.5118" y1="-1.143" x2="-5.5118" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="21.463" y1="7.62" x2="26.543" y2="7.62" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="11.684" x2="-18.796" y2="11.684" width="0.1524" layer="21"/>
<wire x1="-19.177" y1="12.319" x2="-19.177" y2="17.399" width="0.1524" layer="21"/>
<wire x1="-19.812" y1="11.684" x2="-19.177" y2="12.319" width="0.1524" layer="21" curve="90"/>
<wire x1="18.796" y1="11.684" x2="26.543" y2="11.684" width="0.1524" layer="21"/>
<wire x1="19.177" y1="12.319" x2="19.177" y2="17.399" width="0.1524" layer="21"/>
<wire x1="19.177" y1="12.319" x2="19.812" y2="11.684" width="0.1524" layer="21" curve="90"/>
<wire x1="-12.4206" y1="1.397" x2="-12.4206" y2="2.413" width="0.8128" layer="51"/>
<wire x1="-15.1892" y1="1.397" x2="-15.1892" y2="2.413" width="0.8128" layer="51"/>
<wire x1="-13.7922" y1="-1.143" x2="-13.7922" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="-16.5608" y1="-1.143" x2="-16.5608" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="9.652" y1="1.397" x2="9.652" y2="2.413" width="0.8128" layer="51"/>
<wire x1="11.049" y1="-1.143" x2="11.049" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="13.7922" y1="-1.143" x2="13.7922" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="16.5608" y1="-1.143" x2="16.5608" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="12.4206" y1="1.397" x2="12.4206" y2="2.413" width="0.8128" layer="51"/>
<wire x1="15.1892" y1="1.397" x2="15.1892" y2="2.413" width="0.8128" layer="51"/>
<wire x1="-18.415" y1="17.907" x2="18.415" y2="17.907" width="0.1524" layer="21"/>
<wire x1="18.415" y1="17.907" x2="18.669" y2="17.907" width="0.1524" layer="21"/>
<circle x="-23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<circle x="-23.5204" y="0" radius="2.667" width="0" layer="42"/>
<circle x="-23.5204" y="0" radius="2.667" width="0" layer="43"/>
<circle x="23.5204" y="0" radius="2.667" width="0" layer="42"/>
<circle x="23.5204" y="0" radius="2.667" width="0" layer="43"/>
<circle x="23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<pad name="1" x="-16.5608" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="2" x="-13.7922" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="3" x="-11.049" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="4" x="-8.2804" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="5" x="-5.5118" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="6" x="-2.7686" y="-1.27" drill="1.016" diameter="1.67" shape="octagon"/>
<pad name="7" x="0" y="-1.27" drill="1.016" diameter="1.67" shape="octagon"/>
<pad name="8" x="2.7686" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="9" x="5.5118" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="10" x="8.2804" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="11" x="11.049" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="12" x="13.7922" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="13" x="16.5608" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="14" x="-15.1892" y="1.27" drill="1.016" shape="octagon"/>
<pad name="15" x="-12.4206" y="1.27" drill="1.016" shape="octagon"/>
<pad name="16" x="-9.652" y="1.27" drill="1.016" shape="octagon"/>
<pad name="17" x="-6.9088" y="1.27" drill="1.016" diameter="1.67" shape="octagon"/>
<pad name="18" x="-4.1402" y="1.27" drill="1.016" diameter="1.67" shape="octagon"/>
<pad name="19" x="-1.3716" y="1.27" drill="1.016" diameter="1.67" shape="octagon"/>
<pad name="20" x="1.3716" y="1.27" drill="1.016" shape="octagon"/>
<pad name="21" x="4.1402" y="1.27" drill="1.016" shape="octagon"/>
<pad name="22" x="6.9088" y="1.27" drill="1.016" shape="octagon"/>
<pad name="23" x="9.652" y="1.27" drill="1.016" shape="octagon"/>
<pad name="24" x="12.4206" y="1.27" drill="1.016" shape="octagon"/>
<pad name="25" x="15.1892" y="1.27" drill="1.016" shape="octagon"/>
<text x="-26.289" y="-6.35" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-8.382" y="13.97" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-18.669" y="-1.905" size="1.27" layer="21" ratio="10">1</text>
<text x="17.78" y="-1.905" size="1.27" layer="21" ratio="10">13</text>
<text x="-19.558" y="0.635" size="1.27" layer="21" ratio="10">14</text>
<text x="17.78" y="0.635" size="1.27" layer="21" ratio="10">25</text>
<text x="26.035" y="2.54" size="1.27" layer="21" ratio="10" rot="R90">2,54</text>
<text x="-18.669" y="8.509" size="1.27" layer="21" ratio="10">F25</text>
<rectangle x1="-20.32" y1="6.858" x2="20.32" y2="7.62" layer="21"/>
<rectangle x1="-26.543" y1="11.176" x2="26.543" y2="11.684" layer="21"/>
<rectangle x1="-16.9672" y1="-0.381" x2="-16.1544" y2="6.858" layer="21"/>
<rectangle x1="-15.5956" y1="2.159" x2="-14.7828" y2="6.858" layer="21"/>
<rectangle x1="-14.1986" y1="-0.381" x2="-13.3858" y2="6.858" layer="21"/>
<rectangle x1="-12.827" y1="2.159" x2="-12.0142" y2="6.858" layer="21"/>
<rectangle x1="-11.4554" y1="-0.381" x2="-10.6426" y2="6.858" layer="21"/>
<rectangle x1="-10.0584" y1="2.159" x2="-9.2456" y2="6.858" layer="21"/>
<rectangle x1="-8.6868" y1="-0.381" x2="-7.874" y2="6.858" layer="21"/>
<rectangle x1="-7.3152" y1="2.159" x2="-6.5024" y2="6.858" layer="21"/>
<rectangle x1="-5.9182" y1="-0.381" x2="-5.1054" y2="6.858" layer="21"/>
<rectangle x1="-4.5466" y1="2.159" x2="-3.7338" y2="6.858" layer="21"/>
<rectangle x1="-3.175" y1="-0.381" x2="-2.3622" y2="6.858" layer="21"/>
<rectangle x1="-1.778" y1="2.159" x2="-0.9652" y2="6.858" layer="21"/>
<rectangle x1="-0.4064" y1="-0.381" x2="0.4064" y2="6.858" layer="21"/>
<rectangle x1="0.9652" y1="2.159" x2="1.778" y2="6.858" layer="21"/>
<rectangle x1="2.3622" y1="-0.381" x2="3.175" y2="6.858" layer="21"/>
<rectangle x1="3.7338" y1="2.159" x2="4.5466" y2="6.858" layer="21"/>
<rectangle x1="5.1054" y1="-0.381" x2="5.9182" y2="6.858" layer="21"/>
<rectangle x1="6.477" y1="2.159" x2="7.2898" y2="6.858" layer="21"/>
<rectangle x1="7.874" y1="-0.381" x2="8.6868" y2="6.858" layer="21"/>
<rectangle x1="9.2456" y1="2.159" x2="10.0584" y2="6.858" layer="21"/>
<rectangle x1="10.6426" y1="-0.381" x2="11.4554" y2="6.858" layer="21"/>
<rectangle x1="12.0142" y1="2.159" x2="12.827" y2="6.858" layer="21"/>
<rectangle x1="13.3858" y1="-0.381" x2="14.1986" y2="6.858" layer="21"/>
<rectangle x1="14.7828" y1="2.159" x2="15.5956" y2="6.858" layer="21"/>
<rectangle x1="16.1544" y1="-0.381" x2="16.9672" y2="6.858" layer="21"/>
<hole x="-23.5204" y="0" drill="3.302"/>
<hole x="23.5204" y="0" drill="3.302"/>
</package>
<package name="F25HP">
<description>&lt;b&gt;SUB-D&lt;/b&gt;</description>
<wire x1="-18.669" y1="17.907" x2="-18.415" y2="17.907" width="0.1524" layer="21"/>
<wire x1="-19.177" y1="17.399" x2="-18.669" y2="17.907" width="0.1524" layer="21" curve="-90"/>
<wire x1="18.669" y1="17.907" x2="19.177" y2="17.399" width="0.1524" layer="21" curve="-90"/>
<wire x1="-18.796" y1="11.684" x2="18.796" y2="11.684" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="7.62" x2="-21.463" y2="7.62" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="7.62" x2="-26.543" y2="10.668" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="7.62" x2="-26.543" y2="7.493" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="10.668" x2="-26.543" y2="11.176" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="11.176" x2="-26.543" y2="11.684" width="0.1524" layer="21"/>
<wire x1="26.543" y1="11.684" x2="26.543" y2="11.176" width="0.1524" layer="21"/>
<wire x1="26.543" y1="11.176" x2="26.543" y2="10.668" width="0.1524" layer="21"/>
<wire x1="26.543" y1="10.668" x2="26.543" y2="7.62" width="0.1524" layer="21"/>
<wire x1="26.543" y1="7.62" x2="26.543" y2="7.493" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="11.176" x2="-24.003" y2="11.176" width="0.1524" layer="21"/>
<wire x1="-24.003" y1="11.176" x2="-21.463" y2="11.176" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="10.668" x2="-21.463" y2="11.176" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="10.668" x2="-21.336" y2="10.668" width="0.1524" layer="21"/>
<wire x1="-21.336" y1="8.255" x2="-20.701" y2="7.62" width="0.1524" layer="21" curve="90"/>
<wire x1="-21.463" y1="7.62" x2="-20.574" y2="7.62" width="0.1524" layer="21"/>
<wire x1="-20.574" y1="7.62" x2="-20.32" y2="7.62" width="0.1524" layer="21"/>
<wire x1="-21.336" y1="8.255" x2="-21.336" y2="10.668" width="0.1524" layer="21"/>
<wire x1="-20.32" y1="7.62" x2="-20.32" y2="6.858" width="0.1524" layer="21"/>
<wire x1="-20.32" y1="7.62" x2="20.32" y2="7.62" width="0.1524" layer="21"/>
<wire x1="-20.32" y1="6.858" x2="20.32" y2="6.858" width="0.1524" layer="21"/>
<wire x1="20.32" y1="7.62" x2="20.32" y2="6.858" width="0.1524" layer="21"/>
<wire x1="20.32" y1="7.62" x2="20.574" y2="7.62" width="0.1524" layer="21"/>
<wire x1="20.574" y1="7.62" x2="21.463" y2="7.62" width="0.1524" layer="21"/>
<wire x1="24.003" y1="11.176" x2="26.543" y2="11.176" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="11.176" x2="21.463" y2="11.176" width="0.1524" layer="21"/>
<wire x1="21.463" y1="11.176" x2="24.003" y2="11.176" width="0.1524" layer="21"/>
<wire x1="21.463" y1="10.668" x2="21.463" y2="11.176" width="0.1524" layer="21"/>
<wire x1="21.336" y1="8.255" x2="21.336" y2="10.668" width="0.1524" layer="21"/>
<wire x1="21.336" y1="10.668" x2="21.463" y2="10.668" width="0.1524" layer="21"/>
<wire x1="20.701" y1="7.62" x2="21.336" y2="8.255" width="0.1524" layer="21" curve="90"/>
<wire x1="-25.908" y1="-3.175" x2="-26.543" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-2.54" x2="-26.543" y2="7.493" width="0.1524" layer="21"/>
<wire x1="-21.082" y1="-3.175" x2="-25.908" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="-21.082" y1="-3.175" x2="-20.574" y2="-2.667" width="0.1524" layer="21"/>
<wire x1="-20.574" y1="-2.667" x2="-20.574" y2="7.62" width="0.1524" layer="21"/>
<wire x1="20.574" y1="-2.667" x2="20.574" y2="7.62" width="0.1524" layer="21"/>
<wire x1="20.574" y1="-2.667" x2="21.082" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="21.082" y1="-3.175" x2="26.035" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="26.035" y1="-3.175" x2="26.543" y2="-2.667" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-2.667" x2="26.543" y2="7.493" width="0.1524" layer="21"/>
<wire x1="-2.7686" y1="-1.143" x2="-2.7686" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="-8.2804" y1="-1.143" x2="-8.2804" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="-11.049" y1="-1.143" x2="-11.049" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="-9.652" y1="1.397" x2="-9.652" y2="2.413" width="0.8128" layer="51"/>
<wire x1="-6.9088" y1="1.397" x2="-6.9088" y2="2.413" width="0.8128" layer="51"/>
<wire x1="-1.3716" y1="1.397" x2="-1.3716" y2="2.413" width="0.8128" layer="51"/>
<wire x1="2.7686" y1="-1.143" x2="2.7686" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="6.9088" y1="1.397" x2="6.9088" y2="2.413" width="0.8128" layer="51"/>
<wire x1="8.2804" y1="-1.143" x2="8.2804" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="4.1402" y1="1.397" x2="4.1402" y2="2.413" width="0.8128" layer="51"/>
<wire x1="5.5118" y1="-1.143" x2="5.5118" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="1.3716" y1="1.397" x2="1.3716" y2="2.413" width="0.8128" layer="51"/>
<wire x1="0" y1="-1.143" x2="0" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="-4.1402" y1="1.397" x2="-4.1402" y2="2.413" width="0.8128" layer="51"/>
<wire x1="-5.5118" y1="-1.143" x2="-5.5118" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="21.463" y1="7.62" x2="26.543" y2="7.62" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="11.684" x2="-18.796" y2="11.684" width="0.1524" layer="21"/>
<wire x1="-19.177" y1="12.319" x2="-19.177" y2="17.399" width="0.1524" layer="21"/>
<wire x1="-19.812" y1="11.684" x2="-19.177" y2="12.319" width="0.1524" layer="21" curve="90"/>
<wire x1="18.796" y1="11.684" x2="26.543" y2="11.684" width="0.1524" layer="21"/>
<wire x1="19.177" y1="12.319" x2="19.177" y2="17.399" width="0.1524" layer="21"/>
<wire x1="19.177" y1="12.319" x2="19.812" y2="11.684" width="0.1524" layer="21" curve="90"/>
<wire x1="-12.4206" y1="1.397" x2="-12.4206" y2="2.413" width="0.8128" layer="51"/>
<wire x1="-15.1892" y1="1.397" x2="-15.1892" y2="2.413" width="0.8128" layer="51"/>
<wire x1="-13.7922" y1="-1.143" x2="-13.7922" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="-16.5608" y1="-1.143" x2="-16.5608" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="9.652" y1="1.397" x2="9.652" y2="2.413" width="0.8128" layer="51"/>
<wire x1="11.049" y1="-1.143" x2="11.049" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="13.7922" y1="-1.143" x2="13.7922" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="16.5608" y1="-1.143" x2="16.5608" y2="-0.127" width="0.8128" layer="51"/>
<wire x1="12.3952" y1="1.397" x2="12.3952" y2="2.413" width="0.8128" layer="51"/>
<wire x1="15.1638" y1="1.397" x2="15.1638" y2="2.413" width="0.8128" layer="51"/>
<wire x1="-18.415" y1="17.907" x2="18.415" y2="17.907" width="0.1524" layer="21"/>
<wire x1="18.415" y1="17.907" x2="18.669" y2="17.907" width="0.1524" layer="21"/>
<circle x="-23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<circle x="23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<pad name="1" x="-16.5608" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="2" x="-13.7922" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="3" x="-11.049" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="4" x="-8.2804" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="5" x="-5.5118" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="6" x="-2.7686" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="7" x="0" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="8" x="2.7686" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="9" x="5.5118" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="10" x="8.2804" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="11" x="11.049" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="12" x="13.7922" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="13" x="16.5608" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="14" x="-15.1892" y="1.27" drill="1.016" shape="octagon"/>
<pad name="15" x="-12.4206" y="1.27" drill="1.016" shape="octagon"/>
<pad name="16" x="-9.652" y="1.27" drill="1.016" shape="octagon"/>
<pad name="17" x="-6.9088" y="1.27" drill="1.016" shape="octagon"/>
<pad name="18" x="-4.1402" y="1.27" drill="1.016" shape="octagon"/>
<pad name="19" x="-1.3716" y="1.27" drill="1.016" shape="octagon"/>
<pad name="20" x="1.3716" y="1.27" drill="1.016" shape="octagon"/>
<pad name="21" x="4.1402" y="1.27" drill="1.016" shape="octagon"/>
<pad name="22" x="6.9088" y="1.27" drill="1.016" shape="octagon"/>
<pad name="23" x="9.652" y="1.27" drill="1.016" shape="octagon"/>
<pad name="24" x="12.4206" y="1.27" drill="1.016" shape="octagon"/>
<pad name="25" x="15.1892" y="1.27" drill="1.016" shape="octagon"/>
<pad name="G1" x="-23.5204" y="0" drill="3.302" diameter="5.08"/>
<pad name="G2" x="23.5204" y="0" drill="3.302" diameter="5.08"/>
<text x="-26.289" y="-6.35" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-8.382" y="13.97" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-18.669" y="-1.905" size="1.27" layer="21" ratio="10">1</text>
<text x="17.78" y="-1.905" size="1.27" layer="21" ratio="10">13</text>
<text x="-19.558" y="0.635" size="1.27" layer="21" ratio="10">14</text>
<text x="17.78" y="0.635" size="1.27" layer="21" ratio="10">25</text>
<text x="26.035" y="2.54" size="1.27" layer="21" ratio="10" rot="R90">2,54</text>
<text x="-18.669" y="8.509" size="1.27" layer="21" ratio="10">F25</text>
<rectangle x1="-20.32" y1="6.858" x2="20.32" y2="7.62" layer="21"/>
<rectangle x1="-26.543" y1="11.176" x2="26.543" y2="11.684" layer="21"/>
<rectangle x1="-16.9672" y1="-0.381" x2="-16.1544" y2="6.858" layer="21"/>
<rectangle x1="-15.5956" y1="2.159" x2="-14.7828" y2="6.858" layer="21"/>
<rectangle x1="-14.1986" y1="-0.381" x2="-13.3858" y2="6.858" layer="21"/>
<rectangle x1="-12.827" y1="2.159" x2="-12.0142" y2="6.858" layer="21"/>
<rectangle x1="-11.4554" y1="-0.381" x2="-10.6426" y2="6.858" layer="21"/>
<rectangle x1="-10.0584" y1="2.159" x2="-9.2456" y2="6.858" layer="21"/>
<rectangle x1="-8.6868" y1="-0.381" x2="-7.874" y2="6.858" layer="21"/>
<rectangle x1="-7.3152" y1="2.159" x2="-6.5024" y2="6.858" layer="21"/>
<rectangle x1="-5.9182" y1="-0.381" x2="-5.1054" y2="6.858" layer="21"/>
<rectangle x1="-4.5466" y1="2.159" x2="-3.7338" y2="6.858" layer="21"/>
<rectangle x1="-3.1496" y1="-0.381" x2="-2.3368" y2="6.858" layer="21"/>
<rectangle x1="-1.778" y1="2.159" x2="-0.9652" y2="6.858" layer="21"/>
<rectangle x1="-0.4064" y1="-0.381" x2="0.4064" y2="6.858" layer="21"/>
<rectangle x1="0.9652" y1="2.159" x2="1.778" y2="6.858" layer="21"/>
<rectangle x1="2.3622" y1="-0.381" x2="3.175" y2="6.858" layer="21"/>
<rectangle x1="3.7338" y1="2.159" x2="4.5466" y2="6.858" layer="21"/>
<rectangle x1="5.1054" y1="-0.381" x2="5.9182" y2="6.858" layer="21"/>
<rectangle x1="6.5024" y1="2.159" x2="7.3152" y2="6.858" layer="21"/>
<rectangle x1="7.874" y1="-0.381" x2="8.6868" y2="6.858" layer="21"/>
<rectangle x1="9.2456" y1="2.159" x2="10.0584" y2="6.858" layer="21"/>
<rectangle x1="10.6426" y1="-0.381" x2="11.4554" y2="6.858" layer="21"/>
<rectangle x1="11.9888" y1="2.159" x2="12.8016" y2="6.858" layer="21"/>
<rectangle x1="13.3858" y1="-0.381" x2="14.1986" y2="6.858" layer="21"/>
<rectangle x1="14.7574" y1="2.159" x2="15.5702" y2="6.858" layer="21"/>
<rectangle x1="16.1544" y1="-0.381" x2="16.9672" y2="6.858" layer="21"/>
</package>
<package name="F25V">
<description>&lt;b&gt;SUB-D&lt;/b&gt;</description>
<wire x1="-18.5674" y1="-2.9464" x2="-19.3548" y2="2.3368" width="0.1524" layer="21"/>
<wire x1="17.272" y1="-3.937" x2="18.5807" y2="-2.905" width="0.1524" layer="21" curve="76.489196"/>
<wire x1="18.034" y1="3.937" x2="19.3495" y2="2.3038" width="0.1524" layer="21" curve="-102.298925"/>
<wire x1="19.3548" y1="2.3114" x2="18.5674" y2="-2.921" width="0.1524" layer="21"/>
<wire x1="-18.034" y1="3.937" x2="18.034" y2="3.937" width="0.1524" layer="21"/>
<wire x1="-19.3541" y1="2.3268" x2="-18.034" y2="3.937" width="0.1524" layer="21" curve="-101.30773"/>
<wire x1="-18.5749" y1="-2.9295" x2="-17.272" y2="-3.937" width="0.1524" layer="21" curve="75.428151"/>
<wire x1="-17.272" y1="-3.937" x2="17.272" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="-6.223" x2="-23.749" y2="-6.096" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="-6.223" x2="-21.209" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="-6.096" x2="-21.209" y2="-6.096" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="-6.096" x2="-21.209" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="-6.223" x2="21.209" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="-25.908" y1="-6.223" x2="-23.749" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-5.588" x2="-25.908" y2="-6.223" width="0.1524" layer="21" curve="90"/>
<wire x1="25.908" y1="-6.223" x2="26.543" y2="-5.588" width="0.1524" layer="21" curve="90"/>
<wire x1="25.908" y1="6.223" x2="23.749" y2="6.223" width="0.1524" layer="21"/>
<wire x1="26.543" y1="5.588" x2="26.543" y2="-5.588" width="0.1524" layer="21"/>
<wire x1="25.908" y1="6.223" x2="26.543" y2="5.588" width="0.1524" layer="21" curve="-90"/>
<wire x1="-26.543" y1="5.588" x2="-26.543" y2="-5.588" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="5.588" x2="-25.908" y2="6.223" width="0.1524" layer="21" curve="-90"/>
<wire x1="21.209" y1="-6.223" x2="21.209" y2="-6.096" width="0.1524" layer="21"/>
<wire x1="21.209" y1="-6.223" x2="23.749" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="21.209" y1="-6.096" x2="23.749" y2="-6.096" width="0.1524" layer="21"/>
<wire x1="23.749" y1="-6.096" x2="23.749" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="23.749" y1="-6.223" x2="25.908" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="21.209" y1="6.223" x2="21.209" y2="6.096" width="0.1524" layer="21"/>
<wire x1="21.209" y1="6.223" x2="-21.209" y2="6.223" width="0.1524" layer="21"/>
<wire x1="21.209" y1="6.096" x2="23.749" y2="6.096" width="0.1524" layer="21"/>
<wire x1="23.749" y1="6.096" x2="23.749" y2="6.223" width="0.1524" layer="21"/>
<wire x1="23.749" y1="6.223" x2="21.209" y2="6.223" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="6.223" x2="-23.749" y2="6.096" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="6.223" x2="-25.908" y2="6.223" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="6.096" x2="-21.209" y2="6.096" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="6.096" x2="-21.209" y2="6.223" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="6.223" x2="-23.749" y2="6.223" width="0.1524" layer="21"/>
<circle x="23.5204" y="0" radius="2.667" width="0" layer="42"/>
<circle x="23.5204" y="0" radius="2.667" width="0" layer="43"/>
<circle x="-23.5204" y="0" radius="2.667" width="0" layer="42"/>
<circle x="-23.5204" y="0" radius="2.667" width="0" layer="43"/>
<circle x="1.3716" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="0" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-1.3716" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<circle x="23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<circle x="2.7686" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-2.7686" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="4.1402" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-4.1402" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="5.5118" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="8.2804" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="6.9088" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="9.652" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="11.049" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="12.4206" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="13.7922" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="15.1892" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="16.5608" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-5.5118" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-8.2804" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-11.049" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-6.9088" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-9.652" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-12.4206" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-13.7922" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-16.5608" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-15.1892" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<pad name="1" x="16.5608" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="2" x="13.7922" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="3" x="11.049" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="4" x="8.2804" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="5" x="5.5118" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="6" x="2.7686" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="7" x="0" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="8" x="-2.7686" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="9" x="-5.5118" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="10" x="-8.2804" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="11" x="-11.049" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="12" x="-13.7922" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="13" x="-16.5608" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="14" x="15.1892" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="15" x="12.4206" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="16" x="9.652" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="17" x="6.9088" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="18" x="4.1402" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="19" x="1.3716" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="20" x="-1.3716" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="21" x="-4.1402" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="22" x="-6.9088" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="23" x="-9.652" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="24" x="-12.4206" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="25" x="-15.1892" y="-1.4224" drill="1.016" shape="octagon"/>
<text x="16.256" y="2.54" size="0.9906" layer="21" ratio="12">1</text>
<text x="13.335" y="2.54" size="0.9906" layer="21" ratio="12">2</text>
<text x="10.668" y="2.54" size="0.9906" layer="21" ratio="12">3</text>
<text x="7.874" y="2.54" size="0.9906" layer="21" ratio="12">4</text>
<text x="5.08" y="2.54" size="0.9906" layer="21" ratio="12">5</text>
<text x="2.286" y="2.54" size="0.9906" layer="21" ratio="12">6</text>
<text x="-0.381" y="2.54" size="0.9906" layer="21" ratio="12">7</text>
<text x="-3.175" y="2.54" size="0.9906" layer="21" ratio="12">8</text>
<text x="-5.969" y="2.54" size="0.9906" layer="21" ratio="12">9</text>
<text x="-26.416" y="6.985" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="6.985" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-9.017" y="2.54" size="0.9906" layer="21" ratio="12">10</text>
<text x="-11.684" y="2.54" size="0.9906" layer="21" ratio="12">11</text>
<text x="-14.478" y="2.54" size="0.9906" layer="21" ratio="12">12</text>
<text x="-17.272" y="2.54" size="0.9906" layer="21" ratio="12">13</text>
<text x="-16.129" y="-3.556" size="0.9906" layer="21" ratio="12">25</text>
<text x="-13.335" y="-3.556" size="0.9906" layer="21" ratio="12">24</text>
<text x="-10.414" y="-3.556" size="0.9906" layer="21" ratio="12">23</text>
<text x="-7.62" y="-3.556" size="0.9906" layer="21" ratio="12">22</text>
<text x="-4.953" y="-3.556" size="0.9906" layer="21" ratio="12">21</text>
<text x="-2.286" y="-3.556" size="0.9906" layer="21" ratio="12">20</text>
<text x="0.635" y="-3.556" size="0.9906" layer="21" ratio="12">19</text>
<text x="3.302" y="-3.556" size="0.9906" layer="21" ratio="12">18</text>
<text x="5.969" y="-3.556" size="0.9906" layer="21" ratio="12">17</text>
<text x="8.763" y="-3.556" size="0.9906" layer="21" ratio="12">16</text>
<text x="11.557" y="-3.556" size="0.9906" layer="21" ratio="12">15</text>
<text x="14.351" y="-3.556" size="0.9906" layer="21" ratio="12">14</text>
<hole x="23.5204" y="0" drill="3.302"/>
<hole x="-23.5204" y="0" drill="3.302"/>
</package>
<package name="F25VP">
<description>&lt;b&gt;SUB-D&lt;/b&gt;</description>
<wire x1="-18.5674" y1="-2.9464" x2="-19.3548" y2="2.3368" width="0.1524" layer="21"/>
<wire x1="17.272" y1="-3.937" x2="18.5807" y2="-2.905" width="0.1524" layer="21" curve="76.489196"/>
<wire x1="18.034" y1="3.937" x2="19.3495" y2="2.3038" width="0.1524" layer="21" curve="-102.298925"/>
<wire x1="19.3548" y1="2.3114" x2="18.5674" y2="-2.921" width="0.1524" layer="21"/>
<wire x1="-18.034" y1="3.937" x2="18.034" y2="3.937" width="0.1524" layer="21"/>
<wire x1="-19.3541" y1="2.3268" x2="-18.034" y2="3.937" width="0.1524" layer="21" curve="-101.30773"/>
<wire x1="-18.5749" y1="-2.9295" x2="-17.272" y2="-3.937" width="0.1524" layer="21" curve="75.428151"/>
<wire x1="-17.272" y1="-3.937" x2="17.272" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="-6.223" x2="-23.749" y2="-6.096" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="-6.223" x2="-21.209" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="-6.096" x2="-21.209" y2="-6.096" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="-6.096" x2="-21.209" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="-6.223" x2="21.209" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="-25.908" y1="-6.223" x2="-23.749" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-5.588" x2="-25.908" y2="-6.223" width="0.1524" layer="21" curve="90"/>
<wire x1="25.908" y1="-6.223" x2="26.543" y2="-5.588" width="0.1524" layer="21" curve="90"/>
<wire x1="25.908" y1="6.223" x2="23.749" y2="6.223" width="0.1524" layer="21"/>
<wire x1="26.543" y1="5.588" x2="26.543" y2="-5.588" width="0.1524" layer="21"/>
<wire x1="25.908" y1="6.223" x2="26.543" y2="5.588" width="0.1524" layer="21" curve="-90"/>
<wire x1="-26.543" y1="5.588" x2="-26.543" y2="-5.588" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="5.588" x2="-25.908" y2="6.223" width="0.1524" layer="21" curve="-90"/>
<wire x1="21.209" y1="-6.223" x2="21.209" y2="-6.096" width="0.1524" layer="21"/>
<wire x1="21.209" y1="-6.223" x2="23.749" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="21.209" y1="-6.096" x2="23.749" y2="-6.096" width="0.1524" layer="21"/>
<wire x1="23.749" y1="-6.096" x2="23.749" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="23.749" y1="-6.223" x2="25.908" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="21.209" y1="6.223" x2="21.209" y2="6.096" width="0.1524" layer="21"/>
<wire x1="21.209" y1="6.223" x2="-21.209" y2="6.223" width="0.1524" layer="21"/>
<wire x1="21.209" y1="6.096" x2="23.749" y2="6.096" width="0.1524" layer="21"/>
<wire x1="23.749" y1="6.096" x2="23.749" y2="6.223" width="0.1524" layer="21"/>
<wire x1="23.749" y1="6.223" x2="21.209" y2="6.223" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="6.223" x2="-23.749" y2="6.096" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="6.223" x2="-25.908" y2="6.223" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="6.096" x2="-21.209" y2="6.096" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="6.096" x2="-21.209" y2="6.223" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="6.223" x2="-23.749" y2="6.223" width="0.1524" layer="21"/>
<circle x="1.3716" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="0" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-1.3716" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<circle x="23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<circle x="2.7686" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-2.7686" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="4.1402" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-4.1402" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="5.5118" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="8.2804" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="6.9088" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="9.652" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="11.049" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="12.4206" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="13.7922" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="15.1892" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="16.5608" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-5.5118" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-8.2804" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-11.049" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-6.9088" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-9.652" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-12.4206" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-13.7922" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-16.5608" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-15.1892" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<pad name="1" x="16.5608" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="2" x="13.7922" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="3" x="11.049" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="4" x="8.2804" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="5" x="5.5118" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="6" x="2.7686" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="7" x="0" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="8" x="-2.7686" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="9" x="-5.5118" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="10" x="-8.2804" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="11" x="-11.049" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="12" x="-13.7922" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="13" x="-16.5608" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="14" x="15.1892" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="15" x="12.4206" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="16" x="9.652" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="17" x="6.9088" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="18" x="4.1402" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="19" x="1.3716" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="20" x="-1.3716" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="21" x="-4.1402" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="22" x="-6.9088" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="23" x="-9.652" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="24" x="-12.4206" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="25" x="-15.1892" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="G1" x="-23.5204" y="0" drill="3.302" diameter="5.08"/>
<pad name="G2" x="23.5204" y="0" drill="3.302" diameter="5.08"/>
<text x="16.256" y="2.54" size="0.9906" layer="21" ratio="12">1</text>
<text x="13.335" y="2.54" size="0.9906" layer="21" ratio="12">2</text>
<text x="10.668" y="2.54" size="0.9906" layer="21" ratio="12">3</text>
<text x="7.874" y="2.54" size="0.9906" layer="21" ratio="12">4</text>
<text x="5.08" y="2.54" size="0.9906" layer="21" ratio="12">5</text>
<text x="2.286" y="2.54" size="0.9906" layer="21" ratio="12">6</text>
<text x="-0.381" y="2.54" size="0.9906" layer="21" ratio="12">7</text>
<text x="-3.175" y="2.54" size="0.9906" layer="21" ratio="12">8</text>
<text x="-5.969" y="2.54" size="0.9906" layer="21" ratio="12">9</text>
<text x="-26.416" y="6.985" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="6.985" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-9.017" y="2.54" size="0.9906" layer="21" ratio="12">10</text>
<text x="-11.684" y="2.54" size="0.9906" layer="21" ratio="12">11</text>
<text x="-14.478" y="2.54" size="0.9906" layer="21" ratio="12">12</text>
<text x="-17.272" y="2.54" size="0.9906" layer="21" ratio="12">13</text>
<text x="-16.129" y="-3.556" size="0.9906" layer="21" ratio="12">25</text>
<text x="-13.335" y="-3.556" size="0.9906" layer="21" ratio="12">24</text>
<text x="-10.414" y="-3.556" size="0.9906" layer="21" ratio="12">23</text>
<text x="-7.62" y="-3.556" size="0.9906" layer="21" ratio="12">22</text>
<text x="-4.953" y="-3.556" size="0.9906" layer="21" ratio="12">21</text>
<text x="-2.286" y="-3.556" size="0.9906" layer="21" ratio="12">20</text>
<text x="0.635" y="-3.556" size="0.9906" layer="21" ratio="12">19</text>
<text x="3.302" y="-3.556" size="0.9906" layer="21" ratio="12">18</text>
<text x="5.969" y="-3.556" size="0.9906" layer="21" ratio="12">17</text>
<text x="8.763" y="-3.556" size="0.9906" layer="21" ratio="12">16</text>
<text x="11.557" y="-3.556" size="0.9906" layer="21" ratio="12">15</text>
<text x="14.351" y="-3.556" size="0.9906" layer="21" ratio="12">14</text>
</package>
<package name="F25VB">
<description>&lt;b&gt;SUB-D&lt;/b&gt;</description>
<wire x1="18.5674" y1="2.9464" x2="19.3548" y2="-2.3368" width="0.1524" layer="21"/>
<wire x1="-18.5807" y1="2.905" x2="-17.272" y2="3.937" width="0.1524" layer="21" curve="-76.489196"/>
<wire x1="-19.3495" y1="-2.3038" x2="-18.034" y2="-3.937" width="0.1524" layer="21" curve="102.298925"/>
<wire x1="-19.3548" y1="-2.3114" x2="-18.5674" y2="2.921" width="0.1524" layer="21"/>
<wire x1="18.034" y1="-3.937" x2="-18.034" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="18.034" y1="-3.937" x2="19.3541" y2="-2.3268" width="0.1524" layer="21" curve="101.30773"/>
<wire x1="17.272" y1="3.937" x2="18.5749" y2="2.9295" width="0.1524" layer="21" curve="-75.428151"/>
<wire x1="17.272" y1="3.937" x2="-17.272" y2="3.937" width="0.1524" layer="21"/>
<wire x1="23.749" y1="6.223" x2="23.749" y2="6.096" width="0.1524" layer="21"/>
<wire x1="23.749" y1="6.223" x2="21.209" y2="6.223" width="0.1524" layer="21"/>
<wire x1="23.749" y1="6.096" x2="21.209" y2="6.096" width="0.1524" layer="21"/>
<wire x1="21.209" y1="6.096" x2="21.209" y2="6.223" width="0.1524" layer="21"/>
<wire x1="21.209" y1="6.223" x2="-21.209" y2="6.223" width="0.1524" layer="21"/>
<wire x1="25.908" y1="6.223" x2="23.749" y2="6.223" width="0.1524" layer="21"/>
<wire x1="25.908" y1="6.223" x2="26.543" y2="5.588" width="0.1524" layer="21" curve="-90"/>
<wire x1="-26.543" y1="5.588" x2="-25.908" y2="6.223" width="0.1524" layer="21" curve="-90"/>
<wire x1="-25.908" y1="-6.223" x2="-23.749" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-5.588" x2="-26.543" y2="5.588" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-5.588" x2="-25.908" y2="-6.223" width="0.1524" layer="21" curve="90"/>
<wire x1="26.543" y1="-5.588" x2="26.543" y2="5.588" width="0.1524" layer="21"/>
<wire x1="25.908" y1="-6.223" x2="26.543" y2="-5.588" width="0.1524" layer="21" curve="90"/>
<wire x1="-21.209" y1="6.223" x2="-21.209" y2="6.096" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="6.223" x2="-23.749" y2="6.223" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="6.096" x2="-23.749" y2="6.096" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="6.096" x2="-23.749" y2="6.223" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="6.223" x2="-25.908" y2="6.223" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="-6.223" x2="-21.209" y2="-6.096" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="-6.223" x2="21.209" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="-21.209" y1="-6.096" x2="-23.749" y2="-6.096" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="-6.096" x2="-23.749" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="-23.749" y1="-6.223" x2="-21.209" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="23.749" y1="-6.223" x2="23.749" y2="-6.096" width="0.1524" layer="21"/>
<wire x1="23.749" y1="-6.223" x2="25.908" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="23.749" y1="-6.096" x2="21.209" y2="-6.096" width="0.1524" layer="21"/>
<wire x1="21.209" y1="-6.096" x2="21.209" y2="-6.223" width="0.1524" layer="21"/>
<wire x1="21.209" y1="-6.223" x2="23.749" y2="-6.223" width="0.1524" layer="21"/>
<circle x="-1.3716" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="0" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="1.3716" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-2.7686" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="2.7686" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-4.1402" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="4.1402" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-5.5118" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-8.2804" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-6.9088" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-9.652" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-11.049" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-12.4206" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-13.7922" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-15.1892" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="-16.5608" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="5.5118" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="8.2804" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="11.049" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="6.9088" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="9.652" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="12.4206" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="13.7922" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="16.5608" y="-1.4224" radius="0.762" width="0.254" layer="51"/>
<circle x="15.1892" y="1.4224" radius="0.762" width="0.254" layer="51"/>
<pad name="1" x="-16.5608" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="2" x="-13.7922" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="3" x="-11.049" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="4" x="-8.2804" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="5" x="-5.5118" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="6" x="-2.7686" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="7" x="0" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="8" x="2.7686" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="9" x="5.5118" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="10" x="8.2804" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="11" x="11.049" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="12" x="13.7922" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="13" x="16.5608" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="14" x="-15.1892" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="15" x="-12.4206" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="16" x="-9.652" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="17" x="-6.9088" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="18" x="-4.1402" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="19" x="-1.3716" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="20" x="1.3716" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="21" x="4.1402" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="22" x="6.9088" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="23" x="9.652" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="24" x="12.4206" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="25" x="15.1892" y="1.4224" drill="1.016" shape="octagon"/>
<text x="-16.256" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">1</text>
<text x="-13.335" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">2</text>
<text x="-10.668" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">3</text>
<text x="-7.874" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">4</text>
<text x="-5.08" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">5</text>
<text x="-2.286" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">6</text>
<text x="0.381" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">7</text>
<text x="3.175" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">8</text>
<text x="5.969" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">9</text>
<text x="-25.146" y="8.255" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-11.43" y="8.255" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="9.017" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">10</text>
<text x="11.684" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">11</text>
<text x="14.478" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">12</text>
<text x="17.272" y="-2.54" size="0.9906" layer="21" ratio="12" rot="R180">13</text>
<text x="16.129" y="3.556" size="0.9906" layer="21" ratio="12" rot="R180">25</text>
<text x="13.335" y="3.556" size="0.9906" layer="21" ratio="12" rot="R180">24</text>
<text x="10.414" y="3.556" size="0.9906" layer="21" ratio="12" rot="R180">23</text>
<text x="7.62" y="3.556" size="0.9906" layer="21" ratio="12" rot="R180">22</text>
<text x="4.953" y="3.556" size="0.9906" layer="21" ratio="12" rot="R180">21</text>
<text x="2.286" y="3.556" size="0.9906" layer="21" ratio="12" rot="R180">20</text>
<text x="-0.635" y="3.556" size="0.9906" layer="21" ratio="12" rot="R180">19</text>
<text x="-3.302" y="3.556" size="0.9906" layer="21" ratio="12" rot="R180">18</text>
<text x="-5.969" y="3.556" size="0.9906" layer="21" ratio="12" rot="R180">17</text>
<text x="-8.763" y="3.556" size="0.9906" layer="21" ratio="12" rot="R180">16</text>
<text x="-11.557" y="3.556" size="0.9906" layer="21" ratio="12" rot="R180">15</text>
<text x="-14.351" y="3.556" size="0.9906" layer="21" ratio="12" rot="R180">14</text>
</package>
</packages>
<symbols>
<symbol name="F25">
<wire x1="-1.651" y1="16.129" x2="-1.651" y2="14.351" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="1.524" y1="14.351" x2="1.524" y2="16.129" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-1.651" y1="13.589" x2="-1.651" y2="11.811" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="1.524" y1="11.811" x2="1.524" y2="13.589" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-1.651" y1="11.049" x2="-1.651" y2="9.271" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="1.524" y1="9.271" x2="1.524" y2="11.049" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-1.651" y1="8.509" x2="-1.651" y2="6.731" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="1.524" y1="6.731" x2="1.524" y2="8.509" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-1.651" y1="5.969" x2="-1.651" y2="4.191" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="1.524" y1="4.191" x2="1.524" y2="5.969" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-1.651" y1="3.429" x2="-1.651" y2="1.651" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="1.524" y1="1.651" x2="1.524" y2="3.429" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-1.651" y1="0.889" x2="-1.651" y2="-0.889" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="1.524" y1="-0.889" x2="1.524" y2="0.889" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-1.651" y1="-1.651" x2="-1.651" y2="-3.429" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="1.524" y1="-3.429" x2="1.524" y2="-1.651" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-1.651" y1="-4.191" x2="-1.651" y2="-5.969" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="1.524" y1="-5.969" x2="1.524" y2="-4.191" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-1.651" y1="-6.731" x2="-1.651" y2="-8.509" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="1.524" y1="-8.509" x2="1.524" y2="-6.731" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-1.651" y1="-9.271" x2="-1.651" y2="-11.049" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="1.524" y1="-11.049" x2="1.524" y2="-9.271" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-1.651" y1="-11.811" x2="-1.651" y2="-13.589" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="1.524" y1="-13.589" x2="1.524" y2="-11.811" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-1.651" y1="-14.351" x2="-1.651" y2="-16.129" width="0.254" layer="94" curve="180" cap="flat"/>
<wire x1="-4.064" y1="17.7262" x2="-2.5226" y2="18.967" width="0.4064" layer="94" curve="-102.324066" cap="flat"/>
<wire x1="-2.5226" y1="18.9668" x2="3.0654" y2="17.7444" width="0.4064" layer="94"/>
<wire x1="3.0654" y1="17.7445" x2="4.0642" y2="16.5038" width="0.4064" layer="94" curve="-77.655139" cap="flat"/>
<wire x1="4.064" y1="-16.5038" x2="4.064" y2="16.5038" width="0.4064" layer="94"/>
<wire x1="3.0654" y1="-17.7444" x2="4.064" y2="-16.5038" width="0.4064" layer="94" curve="77.657889"/>
<wire x1="-4.064" y1="-17.7262" x2="-4.064" y2="17.7262" width="0.4064" layer="94"/>
<wire x1="-2.5226" y1="-18.9668" x2="3.0654" y2="-17.7444" width="0.4064" layer="94"/>
<wire x1="-4.064" y1="-17.7262" x2="-2.5226" y2="-18.9669" width="0.4064" layer="94" curve="102.337599" cap="flat"/>
<text x="-3.81" y="-21.59" size="1.778" layer="96">&gt;VALUE</text>
<text x="-3.81" y="19.685" size="1.778" layer="95">&gt;NAME</text>
<pin name="1" x="-7.62" y="15.24" visible="pad" length="middle" direction="pas"/>
<pin name="14" x="7.62" y="15.24" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="2" x="-7.62" y="12.7" visible="pad" length="middle" direction="pas"/>
<pin name="15" x="7.62" y="12.7" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="3" x="-7.62" y="10.16" visible="pad" length="middle" direction="pas"/>
<pin name="16" x="7.62" y="10.16" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="4" x="-7.62" y="7.62" visible="pad" length="middle" direction="pas"/>
<pin name="17" x="7.62" y="7.62" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="5" x="-7.62" y="5.08" visible="pad" length="middle" direction="pas"/>
<pin name="18" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="6" x="-7.62" y="2.54" visible="pad" length="middle" direction="pas"/>
<pin name="19" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="7" x="-7.62" y="0" visible="pad" length="middle" direction="pas"/>
<pin name="20" x="7.62" y="0" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="8" x="-7.62" y="-2.54" visible="pad" length="middle" direction="pas"/>
<pin name="21" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="9" x="-7.62" y="-5.08" visible="pad" length="middle" direction="pas"/>
<pin name="22" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="10" x="-7.62" y="-7.62" visible="pad" length="middle" direction="pas"/>
<pin name="23" x="7.62" y="-7.62" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="11" x="-7.62" y="-10.16" visible="pad" length="middle" direction="pas"/>
<pin name="24" x="7.62" y="-10.16" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="12" x="-7.62" y="-12.7" visible="pad" length="middle" direction="pas"/>
<pin name="25" x="7.62" y="-12.7" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="13" x="-7.62" y="-15.24" visible="pad" length="middle" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="F25" prefix="X" uservalue="yes">
<description>&lt;b&gt;SUB-D&lt;/b&gt;</description>
<gates>
<gate name="-1" symbol="F25" x="0" y="0"/>
</gates>
<devices>
<device name="D" package="F25D">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-1" pin="10" pad="10"/>
<connect gate="-1" pin="11" pad="11"/>
<connect gate="-1" pin="12" pad="12"/>
<connect gate="-1" pin="13" pad="13"/>
<connect gate="-1" pin="14" pad="14"/>
<connect gate="-1" pin="15" pad="15"/>
<connect gate="-1" pin="16" pad="16"/>
<connect gate="-1" pin="17" pad="17"/>
<connect gate="-1" pin="18" pad="18"/>
<connect gate="-1" pin="19" pad="19"/>
<connect gate="-1" pin="2" pad="2"/>
<connect gate="-1" pin="20" pad="20"/>
<connect gate="-1" pin="21" pad="21"/>
<connect gate="-1" pin="22" pad="22"/>
<connect gate="-1" pin="23" pad="23"/>
<connect gate="-1" pin="24" pad="24"/>
<connect gate="-1" pin="25" pad="25"/>
<connect gate="-1" pin="3" pad="3"/>
<connect gate="-1" pin="4" pad="4"/>
<connect gate="-1" pin="5" pad="5"/>
<connect gate="-1" pin="6" pad="6"/>
<connect gate="-1" pin="7" pad="7"/>
<connect gate="-1" pin="8" pad="8"/>
<connect gate="-1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
<device name="H" package="F25H">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-1" pin="10" pad="10"/>
<connect gate="-1" pin="11" pad="11"/>
<connect gate="-1" pin="12" pad="12"/>
<connect gate="-1" pin="13" pad="13"/>
<connect gate="-1" pin="14" pad="14"/>
<connect gate="-1" pin="15" pad="15"/>
<connect gate="-1" pin="16" pad="16"/>
<connect gate="-1" pin="17" pad="17"/>
<connect gate="-1" pin="18" pad="18"/>
<connect gate="-1" pin="19" pad="19"/>
<connect gate="-1" pin="2" pad="2"/>
<connect gate="-1" pin="20" pad="20"/>
<connect gate="-1" pin="21" pad="21"/>
<connect gate="-1" pin="22" pad="22"/>
<connect gate="-1" pin="23" pad="23"/>
<connect gate="-1" pin="24" pad="24"/>
<connect gate="-1" pin="25" pad="25"/>
<connect gate="-1" pin="3" pad="3"/>
<connect gate="-1" pin="4" pad="4"/>
<connect gate="-1" pin="5" pad="5"/>
<connect gate="-1" pin="6" pad="6"/>
<connect gate="-1" pin="7" pad="7"/>
<connect gate="-1" pin="8" pad="8"/>
<connect gate="-1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
<device name="HP" package="F25HP">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-1" pin="10" pad="10"/>
<connect gate="-1" pin="11" pad="11"/>
<connect gate="-1" pin="12" pad="12"/>
<connect gate="-1" pin="13" pad="13"/>
<connect gate="-1" pin="14" pad="14"/>
<connect gate="-1" pin="15" pad="15"/>
<connect gate="-1" pin="16" pad="16"/>
<connect gate="-1" pin="17" pad="17"/>
<connect gate="-1" pin="18" pad="18"/>
<connect gate="-1" pin="19" pad="19"/>
<connect gate="-1" pin="2" pad="2"/>
<connect gate="-1" pin="20" pad="20"/>
<connect gate="-1" pin="21" pad="21"/>
<connect gate="-1" pin="22" pad="22"/>
<connect gate="-1" pin="23" pad="23"/>
<connect gate="-1" pin="24" pad="24"/>
<connect gate="-1" pin="25" pad="25"/>
<connect gate="-1" pin="3" pad="3"/>
<connect gate="-1" pin="4" pad="4"/>
<connect gate="-1" pin="5" pad="5"/>
<connect gate="-1" pin="6" pad="6"/>
<connect gate="-1" pin="7" pad="7"/>
<connect gate="-1" pin="8" pad="8"/>
<connect gate="-1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
<device name="V" package="F25V">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-1" pin="10" pad="10"/>
<connect gate="-1" pin="11" pad="11"/>
<connect gate="-1" pin="12" pad="12"/>
<connect gate="-1" pin="13" pad="13"/>
<connect gate="-1" pin="14" pad="14"/>
<connect gate="-1" pin="15" pad="15"/>
<connect gate="-1" pin="16" pad="16"/>
<connect gate="-1" pin="17" pad="17"/>
<connect gate="-1" pin="18" pad="18"/>
<connect gate="-1" pin="19" pad="19"/>
<connect gate="-1" pin="2" pad="2"/>
<connect gate="-1" pin="20" pad="20"/>
<connect gate="-1" pin="21" pad="21"/>
<connect gate="-1" pin="22" pad="22"/>
<connect gate="-1" pin="23" pad="23"/>
<connect gate="-1" pin="24" pad="24"/>
<connect gate="-1" pin="25" pad="25"/>
<connect gate="-1" pin="3" pad="3"/>
<connect gate="-1" pin="4" pad="4"/>
<connect gate="-1" pin="5" pad="5"/>
<connect gate="-1" pin="6" pad="6"/>
<connect gate="-1" pin="7" pad="7"/>
<connect gate="-1" pin="8" pad="8"/>
<connect gate="-1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
<device name="VP" package="F25VP">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-1" pin="10" pad="10"/>
<connect gate="-1" pin="11" pad="11"/>
<connect gate="-1" pin="12" pad="12"/>
<connect gate="-1" pin="13" pad="13"/>
<connect gate="-1" pin="14" pad="14"/>
<connect gate="-1" pin="15" pad="15"/>
<connect gate="-1" pin="16" pad="16"/>
<connect gate="-1" pin="17" pad="17"/>
<connect gate="-1" pin="18" pad="18"/>
<connect gate="-1" pin="19" pad="19"/>
<connect gate="-1" pin="2" pad="2"/>
<connect gate="-1" pin="20" pad="20"/>
<connect gate="-1" pin="21" pad="21"/>
<connect gate="-1" pin="22" pad="22"/>
<connect gate="-1" pin="23" pad="23"/>
<connect gate="-1" pin="24" pad="24"/>
<connect gate="-1" pin="25" pad="25"/>
<connect gate="-1" pin="3" pad="3"/>
<connect gate="-1" pin="4" pad="4"/>
<connect gate="-1" pin="5" pad="5"/>
<connect gate="-1" pin="6" pad="6"/>
<connect gate="-1" pin="7" pad="7"/>
<connect gate="-1" pin="8" pad="8"/>
<connect gate="-1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
<device name="VB" package="F25VB">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-1" pin="10" pad="10"/>
<connect gate="-1" pin="11" pad="11"/>
<connect gate="-1" pin="12" pad="12"/>
<connect gate="-1" pin="13" pad="13"/>
<connect gate="-1" pin="14" pad="14"/>
<connect gate="-1" pin="15" pad="15"/>
<connect gate="-1" pin="16" pad="16"/>
<connect gate="-1" pin="17" pad="17"/>
<connect gate="-1" pin="18" pad="18"/>
<connect gate="-1" pin="19" pad="19"/>
<connect gate="-1" pin="2" pad="2"/>
<connect gate="-1" pin="20" pad="20"/>
<connect gate="-1" pin="21" pad="21"/>
<connect gate="-1" pin="22" pad="22"/>
<connect gate="-1" pin="23" pad="23"/>
<connect gate="-1" pin="24" pad="24"/>
<connect gate="-1" pin="25" pad="25"/>
<connect gate="-1" pin="3" pad="3"/>
<connect gate="-1" pin="4" pad="4"/>
<connect gate="-1" pin="5" pad="5"/>
<connect gate="-1" pin="6" pad="6"/>
<connect gate="-1" pin="7" pad="7"/>
<connect gate="-1" pin="8" pad="8"/>
<connect gate="-1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="switch-reed">
<description>&lt;b&gt;Reed switsches&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="MK15">
<description>&lt;b&gt;Reed Sensors for SMD Mounting&lt;/b&gt;&lt;p&gt;
MEDER electronic&lt;br&gt;
Source: &lt;a href="http://www.farnell.com/datasheets/34737.pdf"&gt; Data sheet &lt;/a&gt;</description>
<wire x1="7.9" y1="1.15" x2="7.9" y2="-1.15" width="0.2032" layer="21"/>
<wire x1="7.9" y1="-1.15" x2="-7.9" y2="-1.15" width="0.2032" layer="21"/>
<wire x1="-7.9" y1="-1.15" x2="-7.9" y2="1.15" width="0.2032" layer="21"/>
<wire x1="-7.9" y1="1.15" x2="7.9" y2="1.15" width="0.2032" layer="21"/>
<smd name="1" x="-9.1" y="0" dx="2" dy="1.3" layer="1"/>
<smd name="2" x="9.1" y="0" dx="2" dy="1.3" layer="1"/>
<text x="-6.985" y="1.905" size="1.27" layer="25">&gt;NAME</text>
<text x="-6.35" y="-0.635" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-9.75" y1="-0.35" x2="-7.9" y2="0.35" layer="51"/>
<rectangle x1="7.9" y1="-0.35" x2="9.75" y2="0.35" layer="51" rot="R180"/>
</package>
<package name="CT10-G4">
<description>&lt;b&gt;CT10 Series Molded Switch&lt;/b&gt;&lt;p&gt;
COTO TECHNOLOGY / www.cotorelay.com&lt;br&gt;
Source: &lt;a href="http://www.farnell.com/datasheets/91368.pdf"&gt; Data sheet &lt;/a&gt;</description>
<wire x1="6.3" y1="1.1" x2="6.3" y2="-1.1" width="0.2032" layer="21"/>
<wire x1="6.3" y1="-1.1" x2="-6.3" y2="-1.1" width="0.2032" layer="21"/>
<wire x1="-6.3" y1="-1.1" x2="-6.3" y2="1.1" width="0.2032" layer="21"/>
<wire x1="-6.3" y1="1.1" x2="6.3" y2="1.1" width="0.2032" layer="21"/>
<smd name="1" x="-9.145" y="0" dx="2" dy="1.3" layer="1"/>
<smd name="2" x="9.145" y="0" dx="2" dy="1.3" layer="1"/>
<text x="-6.1802" y="1.3302" size="1.27" layer="25">&gt;NAME</text>
<text x="-6.1802" y="-3.1082" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-9.7" y1="-0.375" x2="-6.4" y2="0.375" layer="51"/>
<rectangle x1="6.4" y1="-0.375" x2="9.7" y2="0.375" layer="51" rot="R180"/>
</package>
<package name="CT10-G1">
<description>&lt;b&gt;CT10 Series Molded Switch&lt;/b&gt;&lt;p&gt;
COTO TECHNOLOGY / www.cotorelay.com&lt;br&gt;
Source: &lt;a href="http://www.farnell.com/datasheets/91368.pdf"&gt; Data sheet &lt;/a&gt;</description>
<wire x1="5.7" y1="1.1" x2="5.7" y2="-1.1" width="0.2032" layer="21"/>
<wire x1="5.7" y1="-1.1" x2="-5.7" y2="-1.1" width="0.2032" layer="21"/>
<wire x1="-5.7" y1="-1.1" x2="-5.7" y2="1.1" width="0.2032" layer="21"/>
<wire x1="-5.7" y1="1.1" x2="5.7" y2="1.1" width="0.2032" layer="21"/>
<smd name="1" x="-7.62" y="0" dx="2" dy="1.3" layer="1"/>
<smd name="2" x="7.62" y="0" dx="2" dy="1.3" layer="1"/>
<text x="-5.5802" y="1.3302" size="1.27" layer="25">&gt;NAME</text>
<text x="-5.5802" y="-3.1082" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-8.175" y1="-0.375" x2="-5.7" y2="0.375" layer="51"/>
<rectangle x1="5.7" y1="-0.375" x2="8.175" y2="0.375" layer="51" rot="R180"/>
</package>
<package name="CT10-A2">
<description>&lt;b&gt;CT10 Series Molded Switch&lt;/b&gt;&lt;p&gt;
COTO TECHNOLOGY / www.cotorelay.com&lt;br&gt;
Source: &lt;a href="http://www.farnell.com/datasheets/91368.pdf"&gt; Data sheet &lt;/a&gt;</description>
<wire x1="5.7" y1="1.1" x2="5.7" y2="-1.1" width="0.2032" layer="21"/>
<wire x1="5.7" y1="-1.1" x2="-5.7" y2="-1.1" width="0.2032" layer="21"/>
<wire x1="-5.7" y1="-1.1" x2="-5.7" y2="1.1" width="0.2032" layer="21"/>
<wire x1="-5.7" y1="1.1" x2="5.7" y2="1.1" width="0.2032" layer="21"/>
<wire x1="-6.25" y1="1.5" x2="6.25" y2="1.5" width="0" layer="20"/>
<wire x1="6.25" y1="1.5" x2="6.25" y2="-1.5" width="0" layer="20"/>
<wire x1="6.25" y1="-1.5" x2="-6.25" y2="-1.5" width="0" layer="20"/>
<wire x1="-6.25" y1="-1.5" x2="-6.25" y2="1.5" width="0" layer="20"/>
<smd name="1" x="-7.5" y="0" dx="2" dy="1.3" layer="1"/>
<smd name="2" x="7.5" y="0" dx="2" dy="1.3" layer="1"/>
<text x="-5.5" y="1.75" size="1.27" layer="25">&gt;NAME</text>
<text x="-5.5" y="-3" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-7.86" y1="-0.505" x2="-5.7" y2="0.505" layer="51"/>
<rectangle x1="5.7" y1="-0.505" x2="7.86" y2="0.505" layer="51" rot="R180"/>
</package>
<package name="MK6-10">
<description>&lt;b&gt;Reed Sonsor for PCB Mountung&lt;/b&gt;&lt;p&gt;
Source: &lt;a href="http://www.farnell.com/datasheets/91181.pdf"&gt; Data sheet &lt;/a&gt;</description>
<wire x1="-14.2" y1="1.8" x2="14.2" y2="1.8" width="0.2032" layer="21"/>
<wire x1="14.2" y1="1.8" x2="14.2" y2="0.2" width="0.2032" layer="21"/>
<wire x1="14.2" y1="0.2" x2="12.2" y2="-1.8" width="0.2032" layer="21" curve="-90"/>
<wire x1="12.2" y1="-1.8" x2="-12.2" y2="-1.8" width="0.2032" layer="21"/>
<wire x1="-12.2" y1="-1.8" x2="-14.2" y2="0.2" width="0.2032" layer="21" curve="-90"/>
<wire x1="-14.2" y1="0.2" x2="-14.2" y2="1.8" width="0.2032" layer="21"/>
<pad name="1" x="-12.7" y="0" drill="0.6" diameter="1.016"/>
<pad name="2" x="12.7" y="0" drill="0.6" diameter="1.016"/>
<text x="-10.16" y="1.905" size="1.27" layer="25">&gt;NAME</text>
<text x="-10.16" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="MK6-8-A">
<description>&lt;b&gt;Reed Sonsor for PCB Mountung&lt;/b&gt;&lt;p&gt;
Source: &lt;a href="http://www.farnell.com/datasheets/91181.pdf"&gt; Data sheet &lt;/a&gt;</description>
<wire x1="-9.505" y1="1.55" x2="9.505" y2="1.55" width="0.2032" layer="21"/>
<wire x1="9.505" y1="1.55" x2="9.505" y2="-1.55" width="0.2032" layer="21" curve="-180"/>
<wire x1="9.505" y1="-1.55" x2="-9.505" y2="-1.55" width="0.2032" layer="21"/>
<wire x1="-9.505" y1="-1.55" x2="-9.505" y2="1.55" width="0.2032" layer="21" curve="-180"/>
<pad name="1" x="-10.16" y="0" drill="0.6" diameter="1.016"/>
<pad name="2" x="10.16" y="0" drill="0.6" diameter="1.016"/>
<text x="-10.16" y="1.905" size="1.27" layer="25">&gt;NAME</text>
<text x="-10.16" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="MK6-7">
<description>&lt;b&gt;Reed Sonsor for PCB Mountung&lt;/b&gt;&lt;p&gt;
Source: &lt;a href="http://www.farnell.com/datasheets/91181.pdf"&gt; Data sheet &lt;/a&gt;</description>
<wire x1="-8.235" y1="1.55" x2="8.235" y2="1.55" width="0.2032" layer="21"/>
<wire x1="8.235" y1="1.55" x2="8.235" y2="-1.55" width="0.2032" layer="21" curve="-180"/>
<wire x1="8.235" y1="-1.55" x2="-8.235" y2="-1.55" width="0.2032" layer="21"/>
<wire x1="-8.235" y1="-1.55" x2="-8.235" y2="1.55" width="0.2032" layer="21" curve="-180"/>
<pad name="1" x="-8.89" y="0" drill="0.6" diameter="1.016"/>
<pad name="2" x="8.89" y="0" drill="0.6" diameter="1.016"/>
<text x="-8.89" y="1.905" size="1.27" layer="25">&gt;NAME</text>
<text x="-8.89" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="MK6-6">
<description>&lt;b&gt;Reed Sonsor for PCB Mountung&lt;/b&gt;&lt;p&gt;
Source: &lt;a href="http://www.farnell.com/datasheets/91181.pdf"&gt; Data sheet &lt;/a&gt;</description>
<wire x1="-6.965" y1="1.55" x2="6.965" y2="1.55" width="0.2032" layer="21"/>
<wire x1="6.965" y1="1.55" x2="6.965" y2="-1.55" width="0.2032" layer="21" curve="-180"/>
<wire x1="6.965" y1="-1.55" x2="-6.965" y2="-1.55" width="0.2032" layer="21"/>
<wire x1="-6.965" y1="-1.55" x2="-6.965" y2="1.55" width="0.2032" layer="21" curve="-180"/>
<pad name="1" x="-7.62" y="0" drill="0.6" diameter="1.016"/>
<pad name="2" x="7.62" y="0" drill="0.6" diameter="1.016"/>
<text x="-7.62" y="1.905" size="1.27" layer="25">&gt;NAME</text>
<text x="-7.62" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="MK6-5">
<description>&lt;b&gt;Reed Sonsor for PCB Mountung&lt;/b&gt;&lt;p&gt;
Source: &lt;a href="http://www.farnell.com/datasheets/91181.pdf"&gt; Data sheet &lt;/a&gt;</description>
<wire x1="-5.545" y1="1.55" x2="5.495" y2="1.55" width="0.2032" layer="21"/>
<wire x1="5.495" y1="1.55" x2="5.495" y2="-1.55" width="0.2032" layer="21" curve="-180"/>
<wire x1="5.495" y1="-1.55" x2="-5.545" y2="-1.55" width="0.2032" layer="21"/>
<wire x1="-5.545" y1="-1.55" x2="-5.545" y2="1.55" width="0.2032" layer="21" curve="-180"/>
<pad name="1" x="-6.35" y="0" drill="0.6" diameter="1.016"/>
<pad name="2" x="6.35" y="0" drill="0.6" diameter="1.016"/>
<text x="-6.35" y="1.905" size="1.27" layer="25">&gt;NAME</text>
<text x="-6.35" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="MK6-4">
<description>&lt;b&gt;Reed Sonsor for PCB Mountung&lt;/b&gt;&lt;p&gt;
Source: &lt;a href="http://www.farnell.com/datasheets/91181.pdf"&gt; Data sheet &lt;/a&gt;</description>
<wire x1="-4.375" y1="1.55" x2="4.375" y2="1.55" width="0.2032" layer="21"/>
<wire x1="4.375" y1="1.55" x2="4.375" y2="-1.55" width="0.2032" layer="21" curve="-180"/>
<wire x1="4.375" y1="-1.55" x2="-4.375" y2="-1.55" width="0.2032" layer="21"/>
<wire x1="-4.375" y1="-1.55" x2="-4.375" y2="1.55" width="0.2032" layer="21" curve="-180"/>
<pad name="1" x="-5.08" y="0" drill="0.6" diameter="1.016"/>
<pad name="2" x="5.08" y="0" drill="0.6" diameter="1.016"/>
<text x="-5.08" y="1.905" size="1.27" layer="25">&gt;NAME</text>
<text x="-5.08" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="REED-SWITCH">
<wire x1="0" y1="-2.54" x2="0" y2="-1.27" width="0.1524" layer="94"/>
<wire x1="0" y1="-1.27" x2="0.635" y2="2.667" width="0.1524" layer="94"/>
<wire x1="0.254" y1="2.54" x2="0" y2="2.54" width="0.1524" layer="94"/>
<wire x1="0" y1="2.54" x2="0" y2="5.08" width="0.1524" layer="94"/>
<wire x1="-1.27" y1="3.81" x2="-1.27" y2="-1.27" width="0.254" layer="94"/>
<wire x1="-1.27" y1="-1.27" x2="1.27" y2="-1.27" width="0.254" layer="94" curve="180"/>
<wire x1="1.27" y1="-1.27" x2="1.27" y2="3.81" width="0.254" layer="94"/>
<wire x1="1.27" y1="3.81" x2="-1.27" y2="3.81" width="0.254" layer="94" curve="180"/>
<wire x1="-3.556" y1="2.54" x2="-2.794" y2="2.54" width="0.1524" layer="94"/>
<wire x1="-2.794" y1="2.54" x2="-2.794" y2="0" width="0.1524" layer="94"/>
<wire x1="-2.794" y1="0" x2="-3.556" y2="0" width="0.1524" layer="94"/>
<wire x1="-3.556" y1="0" x2="-3.556" y2="2.54" width="0.1524" layer="94"/>
<wire x1="-2.54" y1="1.27" x2="-1.651" y2="1.27" width="0.0508" layer="94"/>
<wire x1="-1.651" y1="1.27" x2="-1.905" y2="1.524" width="0.0508" layer="94"/>
<wire x1="-1.651" y1="1.27" x2="-1.905" y2="1.016" width="0.0508" layer="94"/>
<wire x1="-2.286" y1="1.524" x2="-2.54" y2="1.27" width="0.0508" layer="94"/>
<wire x1="-2.286" y1="1.016" x2="-2.54" y2="1.27" width="0.0508" layer="94"/>
<text x="-3.429" y="2.794" size="0.4064" layer="94">N</text>
<text x="-3.429" y="-0.635" size="0.4064" layer="94">S</text>
<text x="2.54" y="2.54" size="1.778" layer="95">&gt;NAME</text>
<text x="2.54" y="0" size="1.778" layer="96">&gt;VALUE</text>
<rectangle x1="-3.556" y1="1.27" x2="-2.794" y2="2.54" layer="94"/>
<pin name="1" x="0" y="7.62" visible="off" length="short" direction="pas" rot="R270"/>
<pin name="2" x="0" y="-5.08" visible="off" length="short" direction="pas" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="REED-SWITCH-" prefix="S">
<description>&lt;b&gt;Reed Sensor&lt;/b&gt;&lt;p&gt;</description>
<gates>
<gate name="G$1" symbol="REED-SWITCH" x="0" y="0"/>
</gates>
<devices>
<device name="MK15-B" package="MK15">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="MEDER" constant="no"/>
<attribute name="MPN" value="MK15-B-2" constant="no"/>
<attribute name="OC_FARNELL" value="1079484" constant="no"/>
</technology>
</technologies>
</device>
<device name="CT10-G4" package="CT10-G4">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
<device name="CT10-1530-G1" package="CT10-G1">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="COTO TECHNOLOGY" constant="no"/>
<attribute name="MPN" value="CT10-1530-G1" constant="no"/>
<attribute name="OC_FARNELL" value="1081696" constant="no"/>
</technology>
</technologies>
</device>
<device name="CT10-A2" package="CT10-A2">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
<device name="MK15-C2" package="MK15">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="MEDER" constant="no"/>
<attribute name="MPN" value="MK15-C-2" constant="no"/>
<attribute name="OC_FARNELL" value="1079485" constant="no"/>
</technology>
</technologies>
</device>
<device name="MK6-10" package="MK6-10">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
<device name="MK6-8-B" package="MK6-8-A">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="MEDER" constant="no"/>
<attribute name="MPN" value="MK6-8-B" constant="no"/>
<attribute name="OC_FARNELL" value="1079481" constant="no"/>
</technology>
</technologies>
</device>
<device name="MK6-7" package="MK6-7">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
<device name="MK6-6" package="MK6-6">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
<device name="MK6-5-B" package="MK6-5">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="MEDER" constant="no"/>
<attribute name="MPN" value="MK6-5-B" constant="no"/>
<attribute name="OC_FARNELL" value="1079479" constant="no"/>
</technology>
</technologies>
</device>
<device name="MK6-4" package="MK6-4">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
<device name="MK6-5-C" package="MK6-5">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="MEDER" constant="no"/>
<attribute name="MPN" value="MK6-5-C" constant="no"/>
<attribute name="OC_FARNELL" value="1079480" constant="no"/>
</technology>
</technologies>
</device>
<device name="MK6-8-H" package="MK6-8-A">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="MEDER" constant="no"/>
<attribute name="MPN" value="MK6-8-H" constant="no"/>
<attribute name="OC_FARNELL" value="1079482" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply1">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
 GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
 Please keep in mind, that these devices are necessary for the
 automatic wiring of the supply signals.&lt;p&gt;
 The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
 In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="GND">
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND" prefix="GND">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="X1" library="con-subd" deviceset="F25" device="H"/>
<part name="S1" library="switch-reed" deviceset="REED-SWITCH-" device="CT10-A2"/>
<part name="GND1" library="supply1" deviceset="GND" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<text x="-10.16" y="93.98" size="1.778" layer="91">TEMPORARY
C6, C4 and C3 will connect to DB25.</text>
</plain>
<instances>
<instance part="X1" gate="-1" x="7.62" y="81.28" rot="R270"/>
<instance part="S1" gate="G$1" x="5.08" y="63.5" rot="R180"/>
<instance part="GND1" gate="1" x="-5.08" y="45.72"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<pinref part="X1" gate="-1" pin="21"/>
<pinref part="S1" gate="G$1" pin="2"/>
<wire x1="5.08" y1="73.66" x2="5.08" y2="68.58" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="X1" gate="-1" pin="25"/>
<wire x1="-5.08" y1="73.66" x2="-5.08" y2="53.34" width="0.1524" layer="91"/>
<wire x1="-5.08" y1="48.26" x2="-5.08" y2="53.34" width="0.1524" layer="91"/>
<wire x1="-5.08" y1="53.34" x2="5.08" y2="53.34" width="0.1524" layer="91"/>
<pinref part="S1" gate="G$1" pin="1"/>
<wire x1="5.08" y1="53.34" x2="5.08" y2="55.88" width="0.1524" layer="91"/>
<pinref part="GND1" gate="1" pin="GND"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="6.3" minversion="6.2.2" severity="warning">
Since Version 6.2.2 text objects can contain more than one line,
which will not be processed correctly with this version.
</note>
</compatibility>
</eagle>
