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
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
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
<symbol name="+5V">
<wire x1="1.27" y1="-1.905" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="-1.27" y2="-1.905" width="0.254" layer="94"/>
<text x="-2.54" y="-5.08" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="+5V" x="0" y="-2.54" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="GND">
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="+5V" prefix="P+">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="+5V" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
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
<library name="led">
<description>&lt;b&gt;LEDs&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;&lt;br&gt;
Extended by Federico Battaglin &lt;author&gt;&amp;lt;federico.rd@fdpinternational.com&amp;gt;&lt;/author&gt; with DUOLED</description>
<packages>
<package name="1206">
<description>&lt;b&gt;CHICAGO MINIATURE LAMP, INC.&lt;/b&gt;&lt;p&gt;
7022X Series SMT LEDs 1206 Package Size</description>
<wire x1="1.55" y1="-0.75" x2="-1.55" y2="-0.75" width="0.1016" layer="51"/>
<wire x1="-1.55" y1="-0.75" x2="-1.55" y2="0.75" width="0.1016" layer="51"/>
<wire x1="-1.55" y1="0.75" x2="1.55" y2="0.75" width="0.1016" layer="51"/>
<wire x1="1.55" y1="0.75" x2="1.55" y2="-0.75" width="0.1016" layer="51"/>
<wire x1="-0.55" y1="-0.5" x2="0.55" y2="-0.5" width="0.1016" layer="21" curve="95.452622"/>
<wire x1="-0.55" y1="-0.5" x2="-0.55" y2="0.5" width="0.1016" layer="51" curve="-84.547378"/>
<wire x1="-0.55" y1="0.5" x2="0.55" y2="0.5" width="0.1016" layer="21" curve="-95.452622"/>
<wire x1="0.55" y1="0.5" x2="0.55" y2="-0.5" width="0.1016" layer="51" curve="-84.547378"/>
<smd name="A" x="-1.422" y="0" dx="1.6" dy="1.803" layer="1"/>
<smd name="C" x="1.422" y="0" dx="1.6" dy="1.803" layer="1"/>
<text x="-1.27" y="1.27" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.27" y="-2.54" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-0.1" y1="-0.1" x2="0.1" y2="0.1" layer="21"/>
<rectangle x1="0.45" y1="-0.7" x2="0.8" y2="-0.45" layer="51"/>
<rectangle x1="0.8" y1="-0.7" x2="0.9" y2="0.5" layer="51"/>
<rectangle x1="0.8" y1="0.55" x2="0.9" y2="0.7" layer="51"/>
<rectangle x1="-0.9" y1="-0.7" x2="-0.8" y2="0.5" layer="51"/>
<rectangle x1="-0.9" y1="0.55" x2="-0.8" y2="0.7" layer="51"/>
<rectangle x1="0.45" y1="-0.7" x2="0.6" y2="-0.45" layer="21"/>
</package>
<package name="LD260">
<description>&lt;B&gt;LED&lt;/B&gt;&lt;p&gt;
5 mm, square, Siemens</description>
<wire x1="-1.27" y1="-1.27" x2="0" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0" y1="-1.27" x2="1.27" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.27" y1="1.27" x2="0" y2="1.27" width="0.1524" layer="21"/>
<wire x1="0" y1="1.27" x2="-1.27" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-1.27" x2="1.27" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="1.27" y1="1.27" x2="1.27" y2="0.889" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.889" x2="1.27" y2="0" width="0.1524" layer="51"/>
<wire x1="1.27" y1="0" x2="1.27" y2="-0.889" width="0.1524" layer="51"/>
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="0.889" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-1.27" x2="-1.27" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-0.889" x2="-1.27" y2="0" width="0.1524" layer="51"/>
<wire x1="-1.27" y1="0" x2="-1.27" y2="0.889" width="0.1524" layer="51"/>
<wire x1="0" y1="1.27" x2="0.9917" y2="0.7934" width="0.1524" layer="21" curve="-51.33923"/>
<wire x1="-0.9917" y1="0.7934" x2="0" y2="1.27" width="0.1524" layer="21" curve="-51.33923"/>
<wire x1="0" y1="-1.27" x2="0.9917" y2="-0.7934" width="0.1524" layer="21" curve="51.33923"/>
<wire x1="-0.9917" y1="-0.7934" x2="0" y2="-1.27" width="0.1524" layer="21" curve="51.33923"/>
<wire x1="0.9558" y1="-0.8363" x2="1.27" y2="0" width="0.1524" layer="51" curve="41.185419"/>
<wire x1="0.9756" y1="0.813" x2="1.2699" y2="0" width="0.1524" layer="51" curve="-39.806332"/>
<wire x1="-1.27" y1="0" x2="-0.9643" y2="-0.8265" width="0.1524" layer="51" curve="40.600331"/>
<wire x1="-1.27" y1="0" x2="-0.9643" y2="0.8265" width="0.1524" layer="51" curve="-40.600331"/>
<wire x1="-0.889" y1="0" x2="0" y2="0.889" width="0.1524" layer="51" curve="-90"/>
<wire x1="-0.508" y1="0" x2="0" y2="0.508" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-0.508" x2="0.508" y2="0" width="0.1524" layer="21" curve="90"/>
<wire x1="0" y1="-0.889" x2="0.889" y2="0" width="0.1524" layer="51" curve="90"/>
<pad name="A" x="-1.27" y="0" drill="0.8128" shape="octagon"/>
<pad name="K" x="1.27" y="0" drill="0.8128" shape="octagon"/>
<text x="-1.2954" y="1.4732" size="1.016" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.27" y="-2.4892" size="1.016" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="1.27" y1="-0.635" x2="2.032" y2="0.635" layer="51"/>
<rectangle x1="1.905" y1="-0.635" x2="2.032" y2="0.635" layer="21"/>
</package>
<package name="LED2X5">
<description>&lt;B&gt;LED&lt;/B&gt;&lt;p&gt;
2 x 5 mm, rectangle</description>
<wire x1="-2.54" y1="-1.27" x2="2.54" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.54" y1="1.27" x2="2.54" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.54" y1="1.27" x2="-2.54" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.27" x2="-2.54" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="0" x2="0.508" y2="0" width="0.1524" layer="51"/>
<wire x1="-0.508" y1="0.381" x2="-0.508" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="-0.508" y1="0.381" x2="0.508" y2="0" width="0.1524" layer="51"/>
<wire x1="0.508" y1="0" x2="1.778" y2="0" width="0.1524" layer="51"/>
<wire x1="0.508" y1="0" x2="-0.508" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="0.508" y1="0.381" x2="0.508" y2="0" width="0.1524" layer="51"/>
<wire x1="0.508" y1="0" x2="0.508" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="0.889" y1="-0.254" x2="1.143" y2="-0.762" width="0.1524" layer="51"/>
<wire x1="1.143" y1="-0.762" x2="1.143" y2="-0.508" width="0.1524" layer="51"/>
<wire x1="1.143" y1="-0.762" x2="0.9398" y2="-0.6096" width="0.1524" layer="51"/>
<wire x1="0.9398" y1="-0.6096" x2="1.143" y2="-0.508" width="0.1524" layer="51"/>
<wire x1="1.397" y1="-0.254" x2="1.651" y2="-0.762" width="0.1524" layer="51"/>
<wire x1="1.651" y1="-0.762" x2="1.651" y2="-0.508" width="0.1524" layer="51"/>
<wire x1="1.651" y1="-0.762" x2="1.4478" y2="-0.6096" width="0.1524" layer="51"/>
<wire x1="1.4478" y1="-0.6096" x2="1.651" y2="-0.508" width="0.1524" layer="51"/>
<pad name="A" x="-1.27" y="0" drill="0.8128" shape="octagon"/>
<pad name="K" x="1.27" y="0" drill="0.8128" shape="octagon"/>
<text x="-2.54" y="1.397" size="1.016" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="-2.413" size="1.016" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="2.159" y1="-1.27" x2="2.413" y2="1.27" layer="21"/>
</package>
<package name="LED3MM">
<description>&lt;B&gt;LED&lt;/B&gt;&lt;p&gt;
3 mm, round</description>
<wire x1="1.5748" y1="-1.27" x2="1.5748" y2="1.27" width="0.254" layer="51"/>
<wire x1="-1.524" y1="0" x2="-1.1708" y2="0.9756" width="0.1524" layer="51" curve="-39.80361"/>
<wire x1="-1.524" y1="0" x2="-1.1391" y2="-1.0125" width="0.1524" layer="51" curve="41.633208"/>
<wire x1="1.1571" y1="0.9918" x2="1.524" y2="0" width="0.1524" layer="51" curve="-40.601165"/>
<wire x1="1.1708" y1="-0.9756" x2="1.524" y2="0" width="0.1524" layer="51" curve="39.80361"/>
<wire x1="0" y1="1.524" x2="1.2401" y2="0.8858" width="0.1524" layer="21" curve="-54.461337"/>
<wire x1="-1.2192" y1="0.9144" x2="0" y2="1.524" width="0.1524" layer="21" curve="-53.130102"/>
<wire x1="0" y1="-1.524" x2="1.203" y2="-0.9356" width="0.1524" layer="21" curve="52.126876"/>
<wire x1="-1.203" y1="-0.9356" x2="0" y2="-1.524" width="0.1524" layer="21" curve="52.126876"/>
<wire x1="-0.635" y1="0" x2="0" y2="0.635" width="0.1524" layer="51" curve="-90"/>
<wire x1="-1.016" y1="0" x2="0" y2="1.016" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-0.635" x2="0.635" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="0" y1="-1.016" x2="1.016" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="0" y1="2.032" x2="1.561" y2="1.3009" width="0.254" layer="21" curve="-50.193108"/>
<wire x1="-1.7929" y1="0.9562" x2="0" y2="2.032" width="0.254" layer="21" curve="-61.926949"/>
<wire x1="0" y1="-2.032" x2="1.5512" y2="-1.3126" width="0.254" layer="21" curve="49.763022"/>
<wire x1="-1.7643" y1="-1.0082" x2="0" y2="-2.032" width="0.254" layer="21" curve="60.255215"/>
<wire x1="-2.032" y1="0" x2="-1.7891" y2="0.9634" width="0.254" layer="51" curve="-28.301701"/>
<wire x1="-2.032" y1="0" x2="-1.7306" y2="-1.065" width="0.254" layer="51" curve="31.60822"/>
<pad name="A" x="-1.27" y="0" drill="0.8128" shape="octagon"/>
<pad name="K" x="1.27" y="0" drill="0.8128" shape="octagon"/>
<text x="1.905" y="0.381" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="1.905" y="-1.651" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="LED5MM">
<description>&lt;B&gt;LED&lt;/B&gt;&lt;p&gt;
5 mm, round</description>
<wire x1="2.54" y1="-1.905" x2="2.54" y2="1.905" width="0.2032" layer="21"/>
<wire x1="2.54" y1="-1.905" x2="2.54" y2="1.905" width="0.254" layer="21" curve="-286.260205"/>
<wire x1="-1.143" y1="0" x2="0" y2="1.143" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-1.143" x2="1.143" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="-1.651" y1="0" x2="0" y2="1.651" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-1.651" x2="1.651" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="-2.159" y1="0" x2="0" y2="2.159" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-2.159" x2="2.159" y2="0" width="0.1524" layer="51" curve="90"/>
<circle x="0" y="0" radius="2.54" width="0.1524" layer="21"/>
<pad name="A" x="-1.27" y="0" drill="0.8128" shape="octagon"/>
<pad name="K" x="1.27" y="0" drill="0.8128" shape="octagon"/>
<text x="3.175" y="0.5334" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="3.2004" y="-1.8034" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="LSU260">
<description>&lt;B&gt;LED&lt;/B&gt;&lt;p&gt;
1 mm, round, Siemens</description>
<wire x1="0" y1="-0.508" x2="-1.143" y2="-0.508" width="0.1524" layer="51"/>
<wire x1="-1.143" y1="-0.508" x2="-1.143" y2="-0.254" width="0.1524" layer="51"/>
<wire x1="-1.143" y1="0.508" x2="0" y2="0.508" width="0.1524" layer="51"/>
<wire x1="-1.143" y1="-0.254" x2="-1.397" y2="-0.254" width="0.1524" layer="51"/>
<wire x1="-1.143" y1="-0.254" x2="-1.143" y2="0.254" width="0.1524" layer="51"/>
<wire x1="-1.397" y1="-0.254" x2="-1.397" y2="0.254" width="0.1524" layer="51"/>
<wire x1="-1.397" y1="0.254" x2="-1.143" y2="0.254" width="0.1524" layer="51"/>
<wire x1="-1.143" y1="0.254" x2="-1.143" y2="0.508" width="0.1524" layer="51"/>
<wire x1="0.508" y1="-0.254" x2="1.397" y2="-0.254" width="0.1524" layer="51"/>
<wire x1="1.397" y1="-0.254" x2="1.397" y2="0.254" width="0.1524" layer="51"/>
<wire x1="1.397" y1="0.254" x2="0.508" y2="0.254" width="0.1524" layer="51"/>
<wire x1="0.381" y1="-0.381" x2="0.254" y2="-0.508" width="0.1524" layer="21"/>
<wire x1="0.254" y1="-0.508" x2="-0.254" y2="-0.508" width="0.1524" layer="21"/>
<wire x1="-0.381" y1="-0.381" x2="-0.254" y2="-0.508" width="0.1524" layer="21"/>
<wire x1="0.381" y1="0.381" x2="0.254" y2="0.508" width="0.1524" layer="21"/>
<wire x1="0.254" y1="0.508" x2="-0.254" y2="0.508" width="0.1524" layer="21"/>
<wire x1="-0.381" y1="0.381" x2="-0.254" y2="0.508" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.254" x2="0.254" y2="0" width="0.1524" layer="21" curve="90"/>
<wire x1="-0.254" y1="0" x2="0" y2="0.254" width="0.1524" layer="21" curve="-90"/>
<wire x1="0.381" y1="-0.381" x2="0.381" y2="0.381" width="0.1524" layer="21" curve="90"/>
<circle x="0" y="0" radius="0.508" width="0.1524" layer="51"/>
<pad name="A" x="-1.27" y="0" drill="0.8128" shape="octagon"/>
<pad name="K" x="1.27" y="0" drill="0.8128" shape="octagon"/>
<text x="-1.2954" y="0.8382" size="1.016" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.27" y="-1.8542" size="1.016" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-1.397" y1="-0.254" x2="-1.143" y2="0.254" layer="51"/>
<rectangle x1="0.508" y1="-0.254" x2="1.397" y2="0.254" layer="51"/>
</package>
<package name="LZR181">
<description>&lt;B&gt;LED BLOCK&lt;/B&gt;&lt;p&gt;
1 LED, Siemens</description>
<wire x1="-1.27" y1="-1.27" x2="1.27" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-1.27" x2="1.27" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="1.27" y1="1.27" x2="1.27" y2="0.889" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.889" x2="1.27" y2="-0.889" width="0.1524" layer="51"/>
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="0.889" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-1.27" x2="-1.27" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-0.889" x2="-1.27" y2="0.889" width="0.1524" layer="51"/>
<wire x1="-0.889" y1="0" x2="0" y2="0.889" width="0.1524" layer="51" curve="-90"/>
<wire x1="-0.508" y1="0" x2="0" y2="0.508" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-0.508" x2="0.508" y2="0" width="0.1524" layer="21" curve="90"/>
<wire x1="0" y1="-0.889" x2="0.889" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="-0.8678" y1="0.7439" x2="0" y2="1.143" width="0.1524" layer="21" curve="-49.396139"/>
<wire x1="0" y1="1.143" x2="0.8678" y2="0.7439" width="0.1524" layer="21" curve="-49.396139"/>
<wire x1="-0.8678" y1="-0.7439" x2="0" y2="-1.143" width="0.1524" layer="21" curve="49.396139"/>
<wire x1="0" y1="-1.143" x2="0.8678" y2="-0.7439" width="0.1524" layer="21" curve="49.396139"/>
<wire x1="0.8678" y1="0.7439" x2="1.143" y2="0" width="0.1524" layer="51" curve="-40.604135"/>
<wire x1="0.8678" y1="-0.7439" x2="1.143" y2="0" width="0.1524" layer="51" curve="40.604135"/>
<wire x1="-1.143" y1="0" x2="-0.8678" y2="0.7439" width="0.1524" layer="51" curve="-40.604135"/>
<wire x1="-1.143" y1="0" x2="-0.8678" y2="-0.7439" width="0.1524" layer="51" curve="40.604135"/>
<wire x1="-1.27" y1="1.27" x2="1.27" y2="1.27" width="0.1524" layer="21"/>
<pad name="A" x="-1.27" y="0" drill="0.8128" shape="octagon"/>
<pad name="K" x="1.27" y="0" drill="0.8128" shape="octagon"/>
<text x="-1.2954" y="1.4732" size="1.016" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.27" y="-2.4892" size="1.016" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="1.27" y1="-0.889" x2="1.524" y2="0.254" layer="51"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.27" y2="0.254" layer="51"/>
</package>
<package name="Q62902-B152">
<description>&lt;b&gt;LED HOLDER&lt;/b&gt;&lt;p&gt;
Siemens</description>
<wire x1="-2.9718" y1="-1.8542" x2="-2.9718" y2="-0.254" width="0.1524" layer="21"/>
<wire x1="-2.9718" y1="-0.254" x2="-2.9718" y2="0.254" width="0.1524" layer="21"/>
<wire x1="-2.9718" y1="0.254" x2="-2.9718" y2="1.8542" width="0.1524" layer="21"/>
<wire x1="2.9718" y1="-1.8542" x2="-2.1082" y2="-1.8542" width="0.1524" layer="21"/>
<wire x1="-2.1082" y1="-1.8542" x2="-2.54" y2="-1.8542" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="1.8542" x2="-2.1082" y2="1.8542" width="0.1524" layer="21"/>
<wire x1="-2.1082" y1="1.8542" x2="2.9718" y2="1.8542" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.8542" x2="-2.54" y2="1.8542" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.8542" x2="-2.9718" y2="-1.8542" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="1.8542" x2="-2.9718" y2="1.8542" width="0.1524" layer="21"/>
<wire x1="-2.9718" y1="0.254" x2="-2.9718" y2="-0.254" width="0.1524" layer="21" curve="180"/>
<wire x1="-1.1486" y1="0.8814" x2="0" y2="1.4478" width="0.1524" layer="21" curve="-52.498642"/>
<wire x1="0" y1="1.4478" x2="1.1476" y2="0.8827" width="0.1524" layer="21" curve="-52.433716"/>
<wire x1="-1.1351" y1="-0.8987" x2="0" y2="-1.4478" width="0.1524" layer="21" curve="51.629985"/>
<wire x1="0" y1="-1.4478" x2="1.1305" y2="-0.9044" width="0.1524" layer="21" curve="51.339172"/>
<wire x1="1.1281" y1="-0.9074" x2="1.4478" y2="0" width="0.1524" layer="51" curve="38.811177"/>
<wire x1="1.1401" y1="0.8923" x2="1.4478" y2="0" width="0.1524" layer="51" curve="-38.048073"/>
<wire x1="-1.4478" y1="0" x2="-1.1305" y2="-0.9044" width="0.1524" layer="51" curve="38.659064"/>
<wire x1="-1.4478" y1="0" x2="-1.1456" y2="0.8853" width="0.1524" layer="51" curve="-37.696376"/>
<wire x1="0" y1="1.7018" x2="1.4674" y2="0.8618" width="0.1524" layer="21" curve="-59.573488"/>
<wire x1="-1.4618" y1="0.8714" x2="0" y2="1.7018" width="0.1524" layer="21" curve="-59.200638"/>
<wire x1="0" y1="-1.7018" x2="1.4571" y2="-0.8793" width="0.1524" layer="21" curve="58.891781"/>
<wire x1="-1.4571" y1="-0.8793" x2="0" y2="-1.7018" width="0.1524" layer="21" curve="58.891781"/>
<wire x1="-1.7018" y1="0" x2="-1.4447" y2="0.8995" width="0.1524" layer="51" curve="-31.907626"/>
<wire x1="-1.7018" y1="0" x2="-1.4502" y2="-0.8905" width="0.1524" layer="51" curve="31.551992"/>
<wire x1="1.4521" y1="0.8874" x2="1.7018" y2="0" width="0.1524" layer="51" curve="-31.429586"/>
<wire x1="1.4459" y1="-0.8975" x2="1.7018" y2="0" width="0.1524" layer="51" curve="31.828757"/>
<wire x1="-2.1082" y1="1.8542" x2="-2.1082" y2="-1.8542" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="0" x2="0" y2="0.635" width="0.1524" layer="21" curve="-90"/>
<wire x1="-1.016" y1="0" x2="0" y2="1.016" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-0.635" x2="0.635" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="0.0539" y1="-1.0699" x2="1.0699" y2="-0.0539" width="0.1524" layer="51" curve="90"/>
<wire x1="2.9718" y1="1.8542" x2="2.9718" y2="-1.8542" width="0.1524" layer="21"/>
<pad name="K" x="-1.27" y="0" drill="0.8128" shape="long" rot="R90"/>
<pad name="A" x="1.27" y="0" drill="0.8128" shape="long" rot="R90"/>
<text x="-1.905" y="2.286" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.905" y="-3.556" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="Q62902-B153">
<description>&lt;b&gt;LED HOLDER&lt;/b&gt;&lt;p&gt;
Siemens</description>
<wire x1="-5.5118" y1="-3.5052" x2="-5.5118" y2="-0.254" width="0.1524" layer="21"/>
<wire x1="-5.5118" y1="-0.254" x2="-5.5118" y2="0.254" width="0.1524" layer="21"/>
<wire x1="-5.5118" y1="0.254" x2="-5.5118" y2="3.5052" width="0.1524" layer="21"/>
<wire x1="5.5118" y1="-3.5052" x2="-4.6482" y2="-3.5052" width="0.1524" layer="21"/>
<wire x1="-4.6482" y1="-3.5052" x2="-5.08" y2="-3.5052" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="3.5052" x2="-4.6482" y2="3.5052" width="0.1524" layer="21"/>
<wire x1="-4.6482" y1="3.5052" x2="5.5118" y2="3.5052" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-3.5052" x2="-5.08" y2="3.5052" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-3.5052" x2="-5.5118" y2="-3.5052" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="3.5052" x2="-5.5118" y2="3.5052" width="0.1524" layer="21"/>
<wire x1="-5.5118" y1="0.254" x2="-5.5118" y2="-0.254" width="0.1524" layer="21" curve="180"/>
<wire x1="-4.6482" y1="3.5052" x2="-4.6482" y2="-3.5052" width="0.1524" layer="21"/>
<wire x1="5.5118" y1="3.5052" x2="5.5118" y2="-3.5052" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="1.905" x2="-2.54" y2="-1.905" width="0.254" layer="21"/>
<wire x1="0" y1="-1.143" x2="1.143" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="-1.143" y1="0" x2="0" y2="1.143" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-1.651" x2="1.651" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="-1.651" y1="0" x2="0" y2="1.651" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-2.159" x2="2.159" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="-2.2129" y1="0.0539" x2="-0.0539" y2="2.2129" width="0.1524" layer="51" curve="-90.010616"/>
<circle x="0" y="0" radius="2.54" width="0.1524" layer="21"/>
<circle x="0" y="0" radius="3.175" width="0.254" layer="21"/>
<pad name="A" x="1.27" y="0" drill="0.8128" shape="long" rot="R90"/>
<pad name="K" x="-1.27" y="0" drill="0.8128" shape="long" rot="R90"/>
<text x="-4.191" y="3.937" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-4.318" y="-5.08" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="Q62902-B155">
<description>&lt;b&gt;LED HOLDER&lt;/b&gt;&lt;p&gt;
Siemens</description>
<wire x1="-1.27" y1="-3.048" x2="-1.27" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="10.033" y1="3.048" x2="2.921" y2="3.048" width="0.1524" layer="21"/>
<wire x1="10.033" y1="3.048" x2="10.033" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-3.048" x2="2.921" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="2.921" y1="-3.048" x2="2.921" y2="3.048" width="0.1524" layer="21"/>
<wire x1="2.921" y1="-3.048" x2="10.033" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="2.921" y1="3.048" x2="-1.27" y2="3.048" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="2.54" x2="-5.207" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="2.54" x2="-1.27" y2="3.048" width="0.1524" layer="21"/>
<wire x1="-5.207" y1="-2.54" x2="-1.27" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-2.54" x2="-1.27" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-5.207" y1="2.54" x2="-5.207" y2="-2.54" width="0.1524" layer="21" curve="180"/>
<wire x1="-6.985" y1="0.635" x2="-6.985" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-6.096" y1="1.397" x2="-6.096" y2="-1.397" width="0.1524" layer="21"/>
<wire x1="-5.207" y1="1.905" x2="-5.207" y2="-1.905" width="0.1524" layer="21"/>
<pad name="K" x="7.62" y="1.27" drill="0.8128" shape="long"/>
<pad name="A" x="7.62" y="-1.27" drill="0.8128" shape="long"/>
<text x="3.302" y="-2.794" size="1.016" layer="21" ratio="14">A+</text>
<text x="3.302" y="1.778" size="1.016" layer="21" ratio="14">K-</text>
<text x="11.684" y="-2.794" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="0.635" y="-4.445" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="2.921" y1="1.016" x2="6.731" y2="1.524" layer="21"/>
<rectangle x1="2.921" y1="-1.524" x2="6.731" y2="-1.016" layer="21"/>
<hole x="0" y="0" drill="0.8128"/>
</package>
<package name="Q62902-B156">
<description>&lt;b&gt;LED HOLDER&lt;/b&gt;&lt;p&gt;
Siemens</description>
<wire x1="2.54" y1="-1.905" x2="2.54" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-1.143" y1="0" x2="0" y2="1.143" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-1.143" x2="1.143" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="-1.651" y1="0" x2="0" y2="1.651" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-1.651" x2="1.651" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="-2.159" y1="0" x2="0" y2="2.159" width="0.1524" layer="51" curve="-90"/>
<wire x1="0.0539" y1="-2.2129" x2="2.2129" y2="-0.0539" width="0.1524" layer="51" curve="90.005308"/>
<wire x1="2.54" y1="3.81" x2="3.81" y2="2.54" width="0.1524" layer="21"/>
<wire x1="2.54" y1="3.81" x2="-3.81" y2="3.81" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-3.81" x2="-3.81" y2="3.81" width="0.1524" layer="21"/>
<wire x1="3.81" y1="2.54" x2="3.81" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-3.81" x2="-2.54" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-3.302" x2="-2.54" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-3.81" x2="2.54" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-3.302" x2="2.54" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-3.302" x2="-2.54" y2="-3.302" width="0.1524" layer="21"/>
<circle x="0" y="0" radius="2.54" width="0.1524" layer="21"/>
<circle x="0" y="0" radius="3.175" width="0.254" layer="21"/>
<pad name="A" x="-1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="K" x="1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<text x="-3.81" y="4.0894" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.7846" y="-5.3594" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<text x="-3.556" y="-3.302" size="1.016" layer="21" ratio="14">+</text>
<text x="2.794" y="-3.302" size="1.016" layer="21" ratio="14">-</text>
</package>
<package name="SFH480">
<description>&lt;B&gt;IR LED&lt;/B&gt;&lt;p&gt;
infrared emitting diode, Infineon
TO-18, lead spacing 2.54 mm, cathode marking&lt;p&gt;
Inifineon</description>
<wire x1="-2.159" y1="1.524" x2="-2.794" y2="2.159" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="2.159" x2="-2.159" y2="2.794" width="0.1524" layer="21"/>
<wire x1="-1.524" y1="2.159" x2="-2.159" y2="2.794" width="0.1524" layer="21"/>
<wire x1="0" y1="1.778" x2="1.5358" y2="0.8959" width="0.1524" layer="21" curve="-59.743278"/>
<wire x1="-1.5358" y1="0.8959" x2="0" y2="1.778" width="0.1524" layer="21" curve="-59.743278"/>
<wire x1="-1.5358" y1="-0.8959" x2="0" y2="-1.778" width="0.1524" layer="21" curve="59.743278"/>
<wire x1="0" y1="-1.778" x2="1.5358" y2="-0.8959" width="0.1524" layer="21" curve="59.743278"/>
<wire x1="1.5142" y1="0.9318" x2="1.778" y2="0" width="0.1524" layer="51" curve="-31.606487"/>
<wire x1="1.5" y1="-0.9546" x2="1.778" y2="0" width="0.1524" layer="51" curve="32.472615"/>
<wire x1="-1.778" y1="0" x2="-1.5142" y2="-0.9318" width="0.1524" layer="51" curve="31.606487"/>
<wire x1="-1.778" y1="0" x2="-1.5" y2="0.9546" width="0.1524" layer="51" curve="-32.472615"/>
<wire x1="-0.635" y1="0" x2="0" y2="0.635" width="0.1524" layer="51" curve="-90"/>
<wire x1="-1.016" y1="0" x2="0" y2="1.016" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-0.635" x2="0.635" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="0.0539" y1="-1.0699" x2="1.0699" y2="-0.0539" width="0.1524" layer="51" curve="90"/>
<circle x="0" y="0" radius="2.667" width="0.1524" layer="21"/>
<circle x="0" y="0" radius="2.413" width="0.254" layer="21"/>
<pad name="K" x="-1.27" y="0" drill="0.8128" shape="long" rot="R90"/>
<pad name="A" x="1.27" y="0" drill="0.8128" shape="long" rot="R90"/>
<text x="-1.27" y="3.048" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.27" y="-4.318" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="SFH482">
<description>&lt;B&gt;IR LED&lt;/B&gt;&lt;p&gt;
infrared emitting diode, Infineon
TO-18, lead spacing 2.54 mm, cathode marking&lt;p&gt;
Inifineon</description>
<wire x1="-2.159" y1="1.524" x2="-2.794" y2="2.159" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="2.159" x2="-2.159" y2="2.794" width="0.1524" layer="21"/>
<wire x1="-1.524" y1="2.159" x2="-2.159" y2="2.794" width="0.1524" layer="21"/>
<wire x1="0" y1="1.778" x2="1.5358" y2="0.8959" width="0.1524" layer="21" curve="-59.743278"/>
<wire x1="-1.5358" y1="0.8959" x2="0" y2="1.778" width="0.1524" layer="21" curve="-59.743278"/>
<wire x1="-1.5358" y1="-0.8959" x2="0" y2="-1.778" width="0.1524" layer="21" curve="59.743278"/>
<wire x1="0" y1="-1.778" x2="1.5358" y2="-0.8959" width="0.1524" layer="21" curve="59.743278"/>
<wire x1="1.5142" y1="0.9318" x2="1.778" y2="0" width="0.1524" layer="51" curve="-31.606487"/>
<wire x1="1.5" y1="-0.9546" x2="1.778" y2="0" width="0.1524" layer="51" curve="32.472615"/>
<wire x1="-1.778" y1="0" x2="-1.5142" y2="-0.9318" width="0.1524" layer="51" curve="31.606487"/>
<wire x1="-1.778" y1="0" x2="-1.5" y2="0.9546" width="0.1524" layer="51" curve="-32.472615"/>
<wire x1="-0.635" y1="0" x2="0" y2="0.635" width="0.1524" layer="51" curve="-90"/>
<wire x1="-1.016" y1="0" x2="0" y2="1.016" width="0.1524" layer="51" curve="-90"/>
<wire x1="0" y1="-0.635" x2="0.635" y2="0" width="0.1524" layer="51" curve="90"/>
<wire x1="0.0539" y1="-1.0699" x2="1.0699" y2="-0.0539" width="0.1524" layer="51" curve="90"/>
<circle x="0" y="0" radius="2.667" width="0.1524" layer="21"/>
<circle x="0" y="0" radius="2.413" width="0.254" layer="21"/>
<pad name="K" x="1.27" y="0" drill="0.8128" shape="long" rot="R90"/>
<pad name="A" x="-1.27" y="0" drill="0.8128" shape="long" rot="R90"/>
<text x="-1.27" y="3.048" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.27" y="-4.318" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="U57X32">
<description>&lt;B&gt;LED&lt;/B&gt;&lt;p&gt;
rectangle, 5.7 x 3.2 mm</description>
<wire x1="-3.175" y1="1.905" x2="3.175" y2="1.905" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-1.905" x2="3.175" y2="1.905" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-1.905" x2="-3.175" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="1.905" x2="-3.175" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-2.667" y1="1.397" x2="2.667" y2="1.397" width="0.1524" layer="21"/>
<wire x1="2.667" y1="-1.397" x2="2.667" y2="1.397" width="0.1524" layer="21"/>
<wire x1="2.667" y1="-1.397" x2="-2.667" y2="-1.397" width="0.1524" layer="21"/>
<wire x1="-2.667" y1="1.397" x2="-2.667" y2="-1.397" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="1.016" x2="2.54" y2="1.016" width="0.1524" layer="51"/>
<wire x1="2.286" y1="1.27" x2="2.286" y2="-1.27" width="0.1524" layer="51"/>
<wire x1="-2.54" y1="0.508" x2="2.54" y2="0.508" width="0.1524" layer="51"/>
<wire x1="-2.54" y1="0" x2="2.54" y2="0" width="0.1524" layer="51"/>
<wire x1="-2.54" y1="-0.508" x2="2.54" y2="-0.508" width="0.1524" layer="51"/>
<wire x1="-2.54" y1="-1.016" x2="2.54" y2="-1.016" width="0.1524" layer="51"/>
<wire x1="-2.286" y1="1.27" x2="-2.286" y2="-1.27" width="0.1524" layer="51"/>
<wire x1="-1.778" y1="1.27" x2="-1.778" y2="-1.27" width="0.1524" layer="51"/>
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="-1.27" width="0.1524" layer="51"/>
<wire x1="-0.762" y1="1.27" x2="-0.762" y2="-1.27" width="0.1524" layer="51"/>
<wire x1="-0.254" y1="1.27" x2="-0.254" y2="-1.27" width="0.1524" layer="51"/>
<wire x1="0.254" y1="1.27" x2="0.254" y2="-1.27" width="0.1524" layer="51"/>
<wire x1="0.762" y1="1.27" x2="0.762" y2="-1.27" width="0.1524" layer="51"/>
<wire x1="1.27" y1="1.27" x2="1.27" y2="-1.27" width="0.1524" layer="51"/>
<wire x1="1.778" y1="1.27" x2="1.778" y2="-1.27" width="0.1524" layer="51"/>
<pad name="A" x="-1.27" y="0" drill="0.8128" shape="long" rot="R90"/>
<pad name="K" x="1.27" y="0" drill="0.8128" shape="long" rot="R90"/>
<text x="3.683" y="0.254" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="3.683" y="-1.524" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="IRL80A">
<description>&lt;B&gt;IR LED&lt;/B&gt;&lt;p&gt;
IR transmitter Siemens</description>
<wire x1="0.889" y1="2.286" x2="0.889" y2="1.778" width="0.1524" layer="21"/>
<wire x1="0.889" y1="1.778" x2="0.889" y2="0.762" width="0.1524" layer="51"/>
<wire x1="0.889" y1="0.762" x2="0.889" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0.889" y1="-0.635" x2="0.889" y2="-1.778" width="0.1524" layer="51"/>
<wire x1="0.889" y1="-1.778" x2="0.889" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="0.889" y1="-2.286" x2="-0.889" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="-0.889" y1="2.286" x2="-0.889" y2="1.778" width="0.1524" layer="21"/>
<wire x1="-0.889" y1="1.778" x2="-0.889" y2="0.762" width="0.1524" layer="51"/>
<wire x1="-0.889" y1="0.762" x2="-0.889" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="-0.889" y1="-0.762" x2="-0.889" y2="-1.778" width="0.1524" layer="51"/>
<wire x1="-0.889" y1="-1.778" x2="-0.889" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="-0.889" y1="2.286" x2="0.889" y2="2.286" width="0.1524" layer="21"/>
<wire x1="-0.889" y1="-0.762" x2="-0.889" y2="0.762" width="0.1524" layer="21" curve="-180"/>
<wire x1="-1.397" y1="0.254" x2="-1.397" y2="-0.254" width="0.0508" layer="21"/>
<wire x1="-1.143" y1="0.508" x2="-1.143" y2="-0.508" width="0.0508" layer="21"/>
<pad name="K" x="0" y="1.27" drill="0.8128" shape="octagon"/>
<pad name="A" x="0" y="-1.27" drill="0.8128" shape="octagon"/>
<text x="1.27" y="0.381" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="1.27" y="-1.651" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="P-LCC-2">
<description>&lt;b&gt;TOPLED® High-optical Power LED (HOP)&lt;/b&gt;&lt;p&gt;
Source: http://www.osram.convergy.de/ ... ls_t675.pdf</description>
<wire x1="-1.4" y1="-1.05" x2="-1.4" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="-1.4" y1="-1.6" x2="-1.1" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="-1.1" y1="-1.6" x2="-0.85" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="-0.85" y1="-1.6" x2="1.1" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="1.1" y1="-1.6" x2="1.4" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="1.4" y1="-1.6" x2="1.4" y2="1.6" width="0.2032" layer="51"/>
<wire x1="1.4" y1="1.6" x2="1.1" y2="1.6" width="0.2032" layer="51"/>
<wire x1="1.1" y1="1.6" x2="-1.1" y2="1.6" width="0.2032" layer="51"/>
<wire x1="-1.1" y1="1.6" x2="-1.4" y2="1.6" width="0.2032" layer="51"/>
<wire x1="-1.1" y1="1.6" x2="-1.1" y2="1.8" width="0.1016" layer="51"/>
<wire x1="-1.1" y1="1.8" x2="1.1" y2="1.8" width="0.1016" layer="51"/>
<wire x1="1.1" y1="1.8" x2="1.1" y2="1.6" width="0.1016" layer="51"/>
<wire x1="-1.1" y1="-1.6" x2="-1.1" y2="-1.8" width="0.1016" layer="51"/>
<wire x1="-1.1" y1="-1.8" x2="1.1" y2="-1.8" width="0.1016" layer="51"/>
<wire x1="1.1" y1="-1.8" x2="1.1" y2="-1.6" width="0.1016" layer="51"/>
<wire x1="-0.85" y1="-1.6" x2="-1.4" y2="-1.05" width="0.2032" layer="51"/>
<wire x1="-1.4" y1="1.6" x2="-1.4" y2="-1.05" width="0.2032" layer="51"/>
<circle x="0" y="0" radius="1.1" width="0.2032" layer="51"/>
<smd name="C" x="0" y="-2.75" dx="4" dy="4" layer="1" stop="no" cream="no"/>
<smd name="A" x="0" y="2.75" dx="4" dy="4" layer="1" stop="no" cream="no"/>
<text x="-2.54" y="-1.905" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="3.81" y="-1.905" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<text x="-0.635" y="2.54" size="1.27" layer="21">A</text>
<text x="-0.635" y="-3.81" size="1.27" layer="21">C</text>
<rectangle x1="-1.3" y1="-2.25" x2="1.3" y2="-0.75" layer="31"/>
<rectangle x1="-1.3" y1="0.75" x2="1.3" y2="2.25" layer="31"/>
<rectangle x1="-0.25" y1="-0.25" x2="0.25" y2="0.25" layer="21"/>
<rectangle x1="-1.4" y1="0.65" x2="1.4" y2="2.35" layer="29"/>
<rectangle x1="-1.4" y1="-2.35" x2="1.4" y2="-0.65" layer="29"/>
</package>
<package name="OSRAM-MINI-TOP-LED">
<description>&lt;b&gt;BLUE LINETM Hyper Mini TOPLED® Hyper-Bright LED&lt;/b&gt;&lt;p&gt;
Source: http://www.osram.convergy.de/ ... LB M676.pdf</description>
<wire x1="-0.6" y1="0.9" x2="-0.6" y2="-0.7" width="0.1016" layer="51"/>
<wire x1="-0.45" y1="-0.9" x2="-0.4" y2="-0.9" width="0.1016" layer="51"/>
<wire x1="-0.4" y1="-0.9" x2="0.6" y2="-0.9" width="0.1016" layer="51"/>
<wire x1="0.6" y1="-0.9" x2="0.6" y2="0.9" width="0.1016" layer="51"/>
<wire x1="0.6" y1="0.9" x2="-0.6" y2="0.9" width="0.1016" layer="51"/>
<wire x1="-0.45" y1="0.95" x2="-0.45" y2="1.1" width="0.1016" layer="51"/>
<wire x1="-0.45" y1="1.1" x2="0.45" y2="1.1" width="0.1016" layer="51"/>
<wire x1="0.45" y1="1.1" x2="0.45" y2="0.95" width="0.1016" layer="51"/>
<wire x1="-0.6" y1="-0.7" x2="-0.4" y2="-0.9" width="0.1016" layer="51"/>
<wire x1="-0.45" y1="-0.9" x2="-0.45" y2="-1.1" width="0.1016" layer="51"/>
<wire x1="-0.45" y1="-1.1" x2="0.45" y2="-1.1" width="0.1016" layer="51"/>
<wire x1="0.45" y1="-1.1" x2="0.45" y2="-0.95" width="0.1016" layer="51"/>
<smd name="A" x="0" y="2.6" dx="4" dy="4" layer="1" stop="no" cream="no"/>
<smd name="C" x="0" y="-2.6" dx="4" dy="4" layer="1" stop="no" cream="no"/>
<text x="-0.635" y="1.905" size="1.27" layer="21">A</text>
<text x="-0.635" y="-3.175" size="1.27" layer="21">C</text>
<text x="-2.54" y="-1.905" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="3.81" y="-1.905" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-0.5" y1="0.6" x2="0.5" y2="1.4" layer="29"/>
<rectangle x1="-0.5" y1="-1.4" x2="0.5" y2="-0.6" layer="29"/>
<rectangle x1="-0.15" y1="-0.6" x2="0.15" y2="-0.3" layer="51"/>
<rectangle x1="-0.45" y1="0.65" x2="0.45" y2="1.35" layer="31"/>
<rectangle x1="-0.45" y1="-1.35" x2="0.45" y2="-0.65" layer="31"/>
</package>
<package name="OSRAM-SIDELED">
<description>&lt;b&gt;Super SIDELED® High-Current LED&lt;/b&gt;&lt;p&gt;
LG A672, LP A672 &lt;br&gt;
Source: http://www.osram.convergy.de/ ... LG_LP_A672.pdf (2004.05.13)</description>
<wire x1="-1.85" y1="-2.05" x2="-1.85" y2="-0.75" width="0.1016" layer="51"/>
<wire x1="-1.85" y1="-0.75" x2="-1.7" y2="-0.75" width="0.1016" layer="51"/>
<wire x1="-1.7" y1="-0.75" x2="-1.7" y2="0.75" width="0.1016" layer="51"/>
<wire x1="-1.7" y1="0.75" x2="-1.85" y2="0.75" width="0.1016" layer="51"/>
<wire x1="-1.85" y1="0.75" x2="-1.85" y2="2.05" width="0.1016" layer="51"/>
<wire x1="-1.85" y1="2.05" x2="0.9" y2="2.05" width="0.1016" layer="51"/>
<wire x1="0.9" y1="2.05" x2="0.9" y2="-2.05" width="0.1016" layer="51"/>
<wire x1="0.9" y1="-2.05" x2="-1.85" y2="-2.05" width="0.1016" layer="51"/>
<wire x1="0.9" y1="-2.05" x2="1.05" y2="-2.05" width="0.1016" layer="51"/>
<wire x1="1.05" y1="-2.05" x2="1.85" y2="-1.85" width="0.1016" layer="51"/>
<wire x1="1.85" y1="-1.85" x2="1.85" y2="1.85" width="0.1016" layer="51"/>
<wire x1="1.85" y1="1.85" x2="1.05" y2="2.05" width="0.1016" layer="51"/>
<wire x1="1.05" y1="2.05" x2="0.9" y2="2.05" width="0.1016" layer="51"/>
<wire x1="1.05" y1="2.05" x2="1.05" y2="-2.05" width="0.1016" layer="51"/>
<wire x1="-0.55" y1="-0.9" x2="-0.55" y2="0.9" width="0.1016" layer="51" curve="-167.319617"/>
<wire x1="-0.55" y1="-0.9" x2="0.85" y2="-1.2" width="0.1016" layer="51" style="shortdash"/>
<wire x1="-0.55" y1="0.9" x2="0.85" y2="1.2" width="0.1016" layer="51" style="shortdash"/>
<smd name="C" x="0" y="-2.5" dx="4" dy="4" layer="1" stop="no" cream="no"/>
<smd name="A" x="0" y="2.5" dx="4" dy="4" layer="1" stop="no" cream="no"/>
<text x="0.635" y="-3.175" size="1.27" layer="21" rot="R90">C</text>
<text x="0.635" y="2.54" size="1.27" layer="21" rot="R90">A</text>
<text x="-2.54" y="-2.54" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="3.81" y="-2.54" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-2.1" y1="-2.2" x2="2.1" y2="-0.4" layer="29"/>
<rectangle x1="-2.1" y1="0.4" x2="2.1" y2="2.2" layer="29"/>
<rectangle x1="-1.9" y1="-2.1" x2="1.9" y2="-0.6" layer="31"/>
<rectangle x1="-1.9" y1="0.6" x2="1.9" y2="2.1" layer="31"/>
<rectangle x1="-1.85" y1="-2.05" x2="-0.7" y2="-1" layer="51"/>
</package>
<package name="SMART-LED">
<description>&lt;b&gt;SmartLEDTM Hyper-Bright LED&lt;/b&gt;&lt;p&gt;
Source: http://www.osram.convergy.de/ ... LA_LO_LS_LY L896.pdf</description>
<wire x1="-0.35" y1="0.6" x2="0.35" y2="0.6" width="0.1016" layer="51" style="shortdash"/>
<wire x1="0.35" y1="0.6" x2="0.35" y2="-0.6" width="0.1016" layer="21" style="shortdash"/>
<wire x1="0.35" y1="-0.6" x2="0.15" y2="-0.6" width="0.1016" layer="51" style="shortdash"/>
<wire x1="0.15" y1="-0.6" x2="-0.35" y2="-0.6" width="0.1016" layer="51" style="shortdash"/>
<wire x1="-0.35" y1="-0.6" x2="-0.35" y2="0.6" width="0.1016" layer="21" style="shortdash"/>
<wire x1="0.35" y1="-0.4" x2="0.15" y2="-0.6" width="0.1016" layer="51" style="shortdash"/>
<smd name="A" x="0" y="0.725" dx="0.35" dy="0.35" layer="1"/>
<smd name="B" x="0" y="-0.725" dx="0.35" dy="0.35" layer="1"/>
<text x="-0.635" y="-0.635" size="1.016" layer="25" rot="R90">&gt;NAME</text>
<text x="1.905" y="-0.635" size="1.016" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-0.15" y1="-0.35" x2="0.15" y2="-0.05" layer="21"/>
<rectangle x1="-0.15" y1="0.6" x2="0.15" y2="0.85" layer="51"/>
<rectangle x1="-0.15" y1="-0.85" x2="0.15" y2="-0.6" layer="51"/>
</package>
<package name="P-LCC-2-TOPLED-RG">
<description>&lt;b&gt;Hyper TOPLED® RG Hyper-Bright LED&lt;/b&gt;&lt;p&gt;
Source: http://www.osram.convergy.de/ ... LA_LO_LS_LY T776.pdf</description>
<wire x1="-1.4" y1="-1.05" x2="-1.4" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="-1.4" y1="-1.6" x2="-1.1" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="-1.1" y1="-1.6" x2="-0.85" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="-0.85" y1="-1.6" x2="1.1" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="1.1" y1="-1.6" x2="1.4" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="1.4" y1="-1.6" x2="1.4" y2="1.6" width="0.2032" layer="51"/>
<wire x1="1.4" y1="1.6" x2="1.1" y2="1.6" width="0.2032" layer="51"/>
<wire x1="1.1" y1="1.6" x2="-1.1" y2="1.6" width="0.2032" layer="51"/>
<wire x1="-1.1" y1="1.6" x2="-1.4" y2="1.6" width="0.2032" layer="51"/>
<wire x1="-1.1" y1="1.6" x2="-1.1" y2="2.45" width="0.1016" layer="51"/>
<wire x1="1.1" y1="2.45" x2="1.1" y2="1.6" width="0.1016" layer="51"/>
<wire x1="-1.1" y1="-1.6" x2="-1.1" y2="-2.45" width="0.1016" layer="51"/>
<wire x1="1.1" y1="-2.45" x2="1.1" y2="-1.6" width="0.1016" layer="51"/>
<wire x1="-0.85" y1="-1.6" x2="-1.4" y2="-1.05" width="0.2032" layer="51"/>
<wire x1="-1.4" y1="1.6" x2="-1.4" y2="-1.05" width="0.2032" layer="51"/>
<circle x="0" y="0" radius="1.1" width="0.2032" layer="21"/>
<smd name="C" x="0" y="-3.5" dx="4" dy="4" layer="1" stop="no" cream="no"/>
<smd name="A" x="0" y="3.5" dx="4" dy="4" layer="1" stop="no" cream="no"/>
<text x="-2.54" y="-1.905" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="3.81" y="-1.905" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<text x="-0.635" y="3.29" size="1.27" layer="21">A</text>
<text x="-0.635" y="-4.56" size="1.27" layer="21">C</text>
<rectangle x1="-1.3" y1="-3" x2="1.3" y2="-1.5" layer="31"/>
<rectangle x1="-1.3" y1="1.5" x2="1.3" y2="3" layer="31"/>
<rectangle x1="-0.25" y1="-0.25" x2="0.25" y2="0.25" layer="21"/>
<rectangle x1="-1.15" y1="2.4" x2="1.15" y2="2.7" layer="51"/>
<rectangle x1="-1.15" y1="-2.7" x2="1.15" y2="-2.4" layer="51"/>
<rectangle x1="-1.5" y1="1.5" x2="1.5" y2="3.2" layer="29"/>
<rectangle x1="-1.5" y1="-3.2" x2="1.5" y2="-1.5" layer="29"/>
<hole x="0" y="0" drill="2.8"/>
</package>
<package name="MICRO-SIDELED">
<description>&lt;b&gt;Hyper Micro SIDELED®&lt;/b&gt;&lt;p&gt;
Source: http://www.osram.convergy.de/ ... LA_LO_LS_LY Y876.pdf</description>
<wire x1="0.65" y1="1.1" x2="-0.1" y2="1.1" width="0.1016" layer="51"/>
<wire x1="-0.1" y1="1.1" x2="-0.35" y2="1" width="0.1016" layer="51"/>
<wire x1="-0.35" y1="1" x2="-0.35" y2="-0.9" width="0.1016" layer="21"/>
<wire x1="-0.35" y1="-0.9" x2="-0.1" y2="-1.1" width="0.1016" layer="51"/>
<wire x1="-0.1" y1="-1.1" x2="0.65" y2="-1.1" width="0.1016" layer="51"/>
<wire x1="0.65" y1="-1.1" x2="0.65" y2="1.1" width="0.1016" layer="21"/>
<wire x1="0.6" y1="0.9" x2="0.25" y2="0.7" width="0.0508" layer="21"/>
<wire x1="0.25" y1="0.7" x2="0.25" y2="-0.7" width="0.0508" layer="21"/>
<wire x1="0.25" y1="-0.7" x2="0.6" y2="-0.9" width="0.0508" layer="21"/>
<smd name="A" x="0" y="1.95" dx="1.6" dy="1.6" layer="1" stop="no" cream="no"/>
<smd name="C" x="0" y="-1.95" dx="1.6" dy="1.6" layer="1" stop="no" cream="no"/>
<text x="-1.27" y="-1.905" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="2.54" y="-1.905" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-0.4" y1="1.1" x2="0.4" y2="1.8" layer="29"/>
<rectangle x1="-0.4" y1="-1.8" x2="0.4" y2="-1.1" layer="29"/>
<rectangle x1="-0.35" y1="-1.75" x2="0.35" y2="-1.15" layer="31"/>
<rectangle x1="-0.35" y1="1.15" x2="0.35" y2="1.75" layer="31"/>
<rectangle x1="-0.125" y1="1.125" x2="0.125" y2="1.75" layer="51"/>
<rectangle x1="-0.125" y1="-1.75" x2="0.125" y2="-1.125" layer="51"/>
</package>
<package name="P-LCC-4">
<description>&lt;b&gt;Power TOPLED®&lt;/b&gt;&lt;p&gt;
Source: http://www.osram.convergy.de/ ... LA_LO_LA_LY E67B.pdf</description>
<wire x1="-1.4" y1="-1.05" x2="-1.4" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="-1.4" y1="-1.6" x2="-1" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="-1" y1="-1.6" x2="-0.85" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="-0.85" y1="-1.6" x2="1" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="1" y1="-1.6" x2="1.4" y2="-1.6" width="0.2032" layer="51"/>
<wire x1="1.4" y1="-1.6" x2="1.4" y2="1.6" width="0.2032" layer="51"/>
<wire x1="1.4" y1="1.6" x2="1.1" y2="1.6" width="0.2032" layer="51"/>
<wire x1="1.1" y1="1.6" x2="-1" y2="1.6" width="0.2032" layer="51"/>
<wire x1="-1" y1="1.6" x2="-1.4" y2="1.6" width="0.2032" layer="51"/>
<wire x1="-1" y1="1.6" x2="-1" y2="1.8" width="0.1016" layer="51"/>
<wire x1="-1" y1="1.8" x2="-0.5" y2="1.8" width="0.1016" layer="51"/>
<wire x1="-0.5" y1="1.8" x2="-0.5" y2="1.65" width="0.1016" layer="51"/>
<wire x1="0.5" y1="1.65" x2="0.5" y2="1.8" width="0.1016" layer="51"/>
<wire x1="0.5" y1="1.8" x2="1.1" y2="1.8" width="0.1016" layer="51"/>
<wire x1="1.1" y1="1.8" x2="1.1" y2="1.6" width="0.1016" layer="51"/>
<wire x1="-1" y1="-1.6" x2="-1" y2="-1.8" width="0.1016" layer="51"/>
<wire x1="-1" y1="-1.8" x2="-0.5" y2="-1.8" width="0.1016" layer="51"/>
<wire x1="-0.5" y1="-1.8" x2="-0.5" y2="-1.65" width="0.1016" layer="51"/>
<wire x1="0.5" y1="-1.65" x2="0.5" y2="-1.8" width="0.1016" layer="51"/>
<wire x1="0.5" y1="-1.8" x2="1" y2="-1.8" width="0.1016" layer="51"/>
<wire x1="1" y1="-1.8" x2="1" y2="-1.6" width="0.1016" layer="51"/>
<wire x1="-0.85" y1="-1.6" x2="-1.4" y2="-1.05" width="0.2032" layer="51"/>
<wire x1="-1.4" y1="1.6" x2="-1.4" y2="-1.05" width="0.2032" layer="51"/>
<circle x="0" y="0" radius="1.1" width="0.2032" layer="51"/>
<smd name="A" x="-2" y="3.15" dx="3.3" dy="4.8" layer="1" stop="no" cream="no"/>
<smd name="C@3" x="2" y="3.15" dx="3.3" dy="4.8" layer="1" stop="no" cream="no"/>
<smd name="C@4" x="2" y="-3.15" dx="3.3" dy="4.8" layer="1" stop="no" cream="no"/>
<smd name="C@1" x="-2" y="-3.15" dx="3.3" dy="4.8" layer="1" stop="no" cream="no"/>
<text x="-3.81" y="-2.54" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="5.08" y="-2.54" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<text x="-1.905" y="-3.81" size="1.27" layer="21">C</text>
<text x="-1.905" y="2.54" size="1.27" layer="21">A</text>
<text x="1.27" y="2.54" size="1.27" layer="21">C</text>
<text x="1.27" y="-3.81" size="1.27" layer="21">C</text>
<rectangle x1="-1.15" y1="0.75" x2="-0.35" y2="1.85" layer="29"/>
<rectangle x1="0.35" y1="0.75" x2="1.15" y2="1.85" layer="29"/>
<rectangle x1="0.35" y1="-1.85" x2="1.15" y2="-0.75" layer="29"/>
<rectangle x1="-1.15" y1="-1.85" x2="-0.35" y2="-0.75" layer="29"/>
<rectangle x1="-1.1" y1="-1.8" x2="-0.4" y2="-0.8" layer="31"/>
<rectangle x1="0.4" y1="-1.8" x2="1.1" y2="-0.8" layer="31"/>
<rectangle x1="0.4" y1="0.8" x2="1.1" y2="1.8" layer="31"/>
<rectangle x1="-1.1" y1="0.8" x2="-0.4" y2="1.8" layer="31"/>
<rectangle x1="-0.2" y1="-0.2" x2="0.2" y2="0.2" layer="21"/>
</package>
<package name="CHIP-LED0603">
<description>&lt;b&gt;Hyper CHIPLED Hyper-Bright LED&lt;/b&gt;&lt;p&gt;
LB Q993&lt;br&gt;
Source: http://www.osram.convergy.de/ ... Lb_q993.pdf</description>
<wire x1="-0.4" y1="0.45" x2="-0.4" y2="-0.45" width="0.1016" layer="51"/>
<wire x1="0.4" y1="0.45" x2="0.4" y2="-0.45" width="0.1016" layer="51"/>
<smd name="C" x="0" y="0.75" dx="0.8" dy="0.8" layer="1"/>
<smd name="A" x="0" y="-0.75" dx="0.8" dy="0.8" layer="1"/>
<text x="-0.635" y="-0.635" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="1.905" y="-0.635" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-0.45" y1="0.45" x2="0.45" y2="0.85" layer="51"/>
<rectangle x1="-0.45" y1="-0.85" x2="0.45" y2="-0.45" layer="51"/>
<rectangle x1="-0.45" y1="0" x2="-0.3" y2="0.3" layer="21"/>
<rectangle x1="0.3" y1="0" x2="0.45" y2="0.3" layer="21"/>
<rectangle x1="-0.15" y1="0" x2="0.15" y2="0.3" layer="21"/>
</package>
<package name="CHIP-LED0805">
<description>&lt;b&gt;Hyper CHIPLED Hyper-Bright LED&lt;/b&gt;&lt;p&gt;
LB R99A&lt;br&gt;
Source: http://www.osram.convergy.de/ ... lb_r99a.pdf</description>
<wire x1="-0.625" y1="0.45" x2="-0.625" y2="-0.45" width="0.1016" layer="51"/>
<wire x1="0.625" y1="0.45" x2="0.625" y2="-0.475" width="0.1016" layer="51"/>
<smd name="C" x="0" y="1.05" dx="1.2" dy="1.2" layer="1"/>
<smd name="A" x="0" y="-1.05" dx="1.2" dy="1.2" layer="1"/>
<text x="-1.27" y="-1.27" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="2.54" y="-1.27" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-0.675" y1="0" x2="-0.525" y2="0.3" layer="21"/>
<rectangle x1="0.525" y1="0" x2="0.675" y2="0.3" layer="21"/>
<rectangle x1="-0.15" y1="0" x2="0.15" y2="0.3" layer="21"/>
<rectangle x1="-0.675" y1="0.45" x2="0.675" y2="1.05" layer="51"/>
<rectangle x1="-0.675" y1="-1.05" x2="0.675" y2="-0.45" layer="51"/>
</package>
<package name="MINI-TOPLED-SANTANA">
<description>&lt;b&gt;Mini TOPLED Santana®&lt;/b&gt;&lt;p&gt;
Source: http://www.osram.convergy.de/ ... LG M470.pdf</description>
<wire x1="0.7" y1="-1" x2="0.35" y2="-1" width="0.1016" layer="21"/>
<wire x1="0.35" y1="-1" x2="-0.7" y2="-1" width="0.1016" layer="21"/>
<wire x1="-0.7" y1="-1" x2="-0.7" y2="1" width="0.1016" layer="21"/>
<wire x1="-0.7" y1="1" x2="0.7" y2="1" width="0.1016" layer="21"/>
<wire x1="0.7" y1="1" x2="0.7" y2="-0.65" width="0.1016" layer="21"/>
<wire x1="0.7" y1="-0.65" x2="0.7" y2="-1" width="0.1016" layer="21"/>
<wire x1="0.45" y1="-0.7" x2="-0.45" y2="-0.7" width="0.1016" layer="21"/>
<wire x1="-0.45" y1="-0.7" x2="-0.45" y2="0.7" width="0.1016" layer="21"/>
<wire x1="-0.45" y1="0.7" x2="0.45" y2="0.7" width="0.1016" layer="21"/>
<wire x1="0.45" y1="0.7" x2="0.45" y2="-0.7" width="0.1016" layer="21"/>
<wire x1="0.7" y1="-0.65" x2="0.35" y2="-1" width="0.1016" layer="21"/>
<smd name="C" x="0" y="-2.2" dx="1.6" dy="1.6" layer="1" stop="no" cream="no"/>
<smd name="A" x="0" y="2.2" dx="1.6" dy="1.6" layer="1" stop="no" cream="no"/>
<text x="-1.27" y="-1.905" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="2.54" y="-1.905" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-0.55" y1="1.5" x2="0.55" y2="2.1" layer="29"/>
<rectangle x1="-0.55" y1="-2.1" x2="0.55" y2="-1.5" layer="29"/>
<rectangle x1="-0.5" y1="-2.05" x2="0.5" y2="-1.55" layer="31"/>
<rectangle x1="-0.5" y1="1.55" x2="0.5" y2="2.05" layer="31"/>
<rectangle x1="-0.2" y1="-0.4" x2="0.15" y2="-0.05" layer="21"/>
<rectangle x1="-0.5" y1="-2.1" x2="0.5" y2="-1.4" layer="51"/>
<rectangle x1="-0.5" y1="1.4" x2="0.5" y2="2.05" layer="51"/>
<rectangle x1="-0.5" y1="1" x2="0.5" y2="1.4" layer="21"/>
<rectangle x1="-0.5" y1="-1.4" x2="0.5" y2="-1.05" layer="21"/>
<hole x="0" y="0" drill="2.7"/>
</package>
<package name="CHIPLED_0805">
<description>&lt;b&gt;CHIPLED&lt;/b&gt;&lt;p&gt;
Source: http://www.osram.convergy.de/ ... LG_R971.pdf</description>
<wire x1="-0.35" y1="0.925" x2="0.35" y2="0.925" width="0.1016" layer="51" curve="162.394521"/>
<wire x1="-0.35" y1="-0.925" x2="0.35" y2="-0.925" width="0.1016" layer="51" curve="-162.394521"/>
<wire x1="0.575" y1="0.525" x2="0.575" y2="-0.525" width="0.1016" layer="51"/>
<wire x1="-0.575" y1="-0.5" x2="-0.575" y2="0.925" width="0.1016" layer="51"/>
<circle x="-0.45" y="0.85" radius="0.103" width="0.1016" layer="51"/>
<smd name="C" x="0" y="1.05" dx="1.2" dy="1.2" layer="1"/>
<smd name="A" x="0" y="-1.05" dx="1.2" dy="1.2" layer="1"/>
<text x="-1.27" y="-1.27" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="2.54" y="-1.27" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="0.3" y1="0.5" x2="0.625" y2="1" layer="51"/>
<rectangle x1="-0.325" y1="0.5" x2="-0.175" y2="0.75" layer="51"/>
<rectangle x1="0.175" y1="0.5" x2="0.325" y2="0.75" layer="51"/>
<rectangle x1="-0.2" y1="0.5" x2="0.2" y2="0.675" layer="51"/>
<rectangle x1="0.3" y1="-1" x2="0.625" y2="-0.5" layer="51"/>
<rectangle x1="-0.625" y1="-1" x2="-0.3" y2="-0.5" layer="51"/>
<rectangle x1="0.175" y1="-0.75" x2="0.325" y2="-0.5" layer="51"/>
<rectangle x1="-0.325" y1="-0.75" x2="-0.175" y2="-0.5" layer="51"/>
<rectangle x1="-0.2" y1="-0.675" x2="0.2" y2="-0.5" layer="51"/>
<rectangle x1="-0.1" y1="0" x2="0.1" y2="0.2" layer="21"/>
<rectangle x1="-0.6" y1="0.5" x2="-0.3" y2="0.8" layer="51"/>
<rectangle x1="-0.625" y1="0.925" x2="-0.3" y2="1" layer="51"/>
</package>
<package name="CHIPLED_1206">
<description>&lt;b&gt;CHIPLED&lt;/b&gt;&lt;p&gt;
Source: http://www.osram.convergy.de/ ... LG_LY N971.pdf</description>
<wire x1="-0.4" y1="1.6" x2="0.4" y2="1.6" width="0.1016" layer="51" curve="172.619069"/>
<wire x1="-0.8" y1="-0.95" x2="-0.8" y2="0.95" width="0.1016" layer="51"/>
<wire x1="0.8" y1="0.95" x2="0.8" y2="-0.95" width="0.1016" layer="51"/>
<circle x="-0.55" y="1.425" radius="0.1" width="0.1016" layer="51"/>
<smd name="C" x="0" y="1.75" dx="1.5" dy="1.5" layer="1"/>
<smd name="A" x="0" y="-1.75" dx="1.5" dy="1.5" layer="1"/>
<text x="-1.27" y="-1.27" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="2.54" y="-1.27" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-0.85" y1="1.525" x2="-0.35" y2="1.65" layer="51"/>
<rectangle x1="-0.85" y1="1.225" x2="-0.625" y2="1.55" layer="51"/>
<rectangle x1="-0.45" y1="1.225" x2="-0.325" y2="1.45" layer="51"/>
<rectangle x1="-0.65" y1="1.225" x2="-0.225" y2="1.35" layer="51"/>
<rectangle x1="0.35" y1="1.3" x2="0.85" y2="1.65" layer="51"/>
<rectangle x1="0.25" y1="1.225" x2="0.85" y2="1.35" layer="51"/>
<rectangle x1="-0.85" y1="0.95" x2="0.85" y2="1.25" layer="51"/>
<rectangle x1="-0.85" y1="-1.65" x2="0.85" y2="-0.95" layer="51"/>
<rectangle x1="-0.85" y1="0.35" x2="-0.525" y2="0.775" layer="21"/>
<rectangle x1="0.525" y1="0.35" x2="0.85" y2="0.775" layer="21"/>
<rectangle x1="-0.175" y1="0" x2="0.175" y2="0.35" layer="21"/>
</package>
<package name="CHIPLED_0603">
<description>&lt;b&gt;CHIPLED&lt;/b&gt;&lt;p&gt;
Source: http://www.osram.convergy.de/ ... LG_LY Q971.pdf</description>
<wire x1="-0.3" y1="0.8" x2="0.3" y2="0.8" width="0.1016" layer="51" curve="170.055574"/>
<wire x1="-0.275" y1="-0.825" x2="0.275" y2="-0.825" width="0.0508" layer="51" curve="-180"/>
<wire x1="-0.4" y1="0.375" x2="-0.4" y2="-0.35" width="0.1016" layer="51"/>
<wire x1="0.4" y1="0.35" x2="0.4" y2="-0.35" width="0.1016" layer="51"/>
<circle x="-0.35" y="0.625" radius="0.075" width="0.0508" layer="51"/>
<smd name="C" x="0" y="0.75" dx="0.8" dy="0.8" layer="1"/>
<smd name="A" x="0" y="-0.75" dx="0.8" dy="0.8" layer="1"/>
<text x="-0.635" y="-1.27" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="1.905" y="-1.27" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-0.45" y1="0.7" x2="-0.25" y2="0.85" layer="51"/>
<rectangle x1="-0.275" y1="0.55" x2="-0.225" y2="0.6" layer="51"/>
<rectangle x1="-0.45" y1="0.35" x2="-0.4" y2="0.725" layer="51"/>
<rectangle x1="0.25" y1="0.55" x2="0.45" y2="0.85" layer="51"/>
<rectangle x1="-0.45" y1="0.35" x2="0.45" y2="0.575" layer="51"/>
<rectangle x1="-0.45" y1="-0.85" x2="-0.25" y2="-0.35" layer="51"/>
<rectangle x1="0.25" y1="-0.85" x2="0.45" y2="-0.35" layer="51"/>
<rectangle x1="-0.275" y1="-0.575" x2="0.275" y2="-0.35" layer="51"/>
<rectangle x1="-0.275" y1="-0.65" x2="-0.175" y2="-0.55" layer="51"/>
<rectangle x1="0.175" y1="-0.65" x2="0.275" y2="-0.55" layer="51"/>
<rectangle x1="-0.125" y1="0" x2="0.125" y2="0.25" layer="21"/>
</package>
<package name="CHIPLED-0603-TTW">
<description>&lt;b&gt;CHIPLED-0603&lt;/b&gt;&lt;p&gt;
Recommended Solder Pad useable for SmartLEDTM and Chipled - Package 0603&lt;br&gt;
Package able to withstand TTW-soldering heat&lt;br&gt;
Package suitable for TTW-soldering&lt;br&gt;
Source: http://www.osram.convergy.de/ ... LO_LS_LY L89K.pdf</description>
<wire x1="-0.3" y1="0.8" x2="0.3" y2="0.8" width="0.1016" layer="51" curve="170.055574"/>
<wire x1="-0.275" y1="-0.825" x2="0.275" y2="-0.825" width="0.0508" layer="51" curve="-180"/>
<wire x1="-0.4" y1="0.375" x2="-0.4" y2="-0.35" width="0.1016" layer="51"/>
<wire x1="0.4" y1="0.35" x2="0.4" y2="-0.35" width="0.1016" layer="51"/>
<circle x="-0.35" y="0.625" radius="0.075" width="0.0508" layer="51"/>
<smd name="C" x="0" y="0.875" dx="0.8" dy="0.5" layer="1" stop="no" cream="no"/>
<smd name="A" x="0" y="-0.875" dx="0.8" dy="0.5" layer="1" stop="no" cream="no"/>
<smd name="A@1" x="0" y="-0.5" dx="0.35" dy="0.35" layer="1"/>
<smd name="C@1" x="0" y="0.5" dx="0.35" dy="0.35" layer="1"/>
<text x="-0.635" y="-1.27" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="1.905" y="-1.27" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-0.45" y1="0.7" x2="-0.25" y2="0.85" layer="51"/>
<rectangle x1="-0.275" y1="0.55" x2="-0.225" y2="0.6" layer="51"/>
<rectangle x1="-0.45" y1="0.35" x2="-0.4" y2="0.725" layer="51"/>
<rectangle x1="0.25" y1="0.55" x2="0.45" y2="0.85" layer="51"/>
<rectangle x1="-0.45" y1="0.35" x2="0.45" y2="0.575" layer="51"/>
<rectangle x1="-0.45" y1="-0.85" x2="-0.25" y2="-0.35" layer="51"/>
<rectangle x1="0.25" y1="-0.85" x2="0.45" y2="-0.35" layer="51"/>
<rectangle x1="-0.275" y1="-0.575" x2="0.275" y2="-0.35" layer="51"/>
<rectangle x1="-0.275" y1="-0.65" x2="-0.175" y2="-0.55" layer="51"/>
<rectangle x1="0.175" y1="-0.65" x2="0.275" y2="-0.55" layer="51"/>
<rectangle x1="-0.125" y1="0" x2="0.125" y2="0.25" layer="21"/>
<rectangle x1="-0.175" y1="0.325" x2="0.175" y2="0.7" layer="29"/>
<rectangle x1="-0.4" y1="0.625" x2="0.4" y2="1.125" layer="29"/>
<rectangle x1="-0.4" y1="-1.125" x2="0.4" y2="-0.625" layer="29"/>
<rectangle x1="-0.175" y1="-0.675" x2="0.175" y2="-0.325" layer="29"/>
</package>
<package name="SMARTLED-TTW">
<description>&lt;b&gt;SmartLED TTW&lt;/b&gt;&lt;p&gt;
Recommended Solder Pad useable for SmartLEDTM and Chipled - Package 0603&lt;br&gt;
Package able to withstand TTW-soldering heat&lt;br&gt;
Package suitable for TTW-soldering&lt;br&gt;
Source: http://www.osram.convergy.de/ ... LO_LS_LY L89K.pdf</description>
<wire x1="-0.35" y1="0.6" x2="0.35" y2="0.6" width="0.1016" layer="51" style="shortdash"/>
<wire x1="0.35" y1="0.6" x2="0.35" y2="-0.6" width="0.1016" layer="21" style="shortdash"/>
<wire x1="0.35" y1="-0.6" x2="0.15" y2="-0.6" width="0.1016" layer="51" style="shortdash"/>
<wire x1="0.15" y1="-0.6" x2="-0.35" y2="-0.6" width="0.1016" layer="51" style="shortdash"/>
<wire x1="-0.35" y1="-0.6" x2="-0.35" y2="0.6" width="0.1016" layer="21" style="shortdash"/>
<wire x1="0.35" y1="-0.4" x2="0.15" y2="-0.6" width="0.1016" layer="51" style="shortdash"/>
<smd name="C" x="0" y="0.875" dx="0.8" dy="0.5" layer="1" stop="no" cream="no"/>
<smd name="A" x="0" y="-0.875" dx="0.8" dy="0.5" layer="1" stop="no" cream="no"/>
<smd name="A@1" x="0" y="-0.5" dx="0.35" dy="0.35" layer="1" stop="no" cream="no"/>
<smd name="C@1" x="0" y="0.5" dx="0.35" dy="0.35" layer="1" stop="no" cream="no"/>
<text x="-0.635" y="-1.27" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="1.905" y="-1.27" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-0.175" y1="0.325" x2="0.175" y2="0.7" layer="29"/>
<rectangle x1="-0.15" y1="-0.35" x2="0.15" y2="-0.05" layer="21"/>
<rectangle x1="-0.15" y1="0.6" x2="0.15" y2="0.85" layer="51"/>
<rectangle x1="-0.15" y1="-0.85" x2="0.15" y2="-0.6" layer="51"/>
<rectangle x1="-0.225" y1="0.3" x2="0.225" y2="0.975" layer="31"/>
<rectangle x1="-0.175" y1="-0.7" x2="0.175" y2="-0.325" layer="29" rot="R180"/>
<rectangle x1="-0.225" y1="-0.975" x2="0.225" y2="-0.3" layer="31" rot="R180"/>
</package>
<package name="LUMILED+">
<description>&lt;b&gt;Lumileds Lighting. LUXEON®&lt;/b&gt; with cool pad&lt;p&gt;
Source: K2.pdf</description>
<wire x1="-3.575" y1="2.3375" x2="-2.3375" y2="3.575" width="0.2032" layer="21"/>
<wire x1="-2.3375" y1="3.575" x2="2.3375" y2="3.575" width="0.2032" layer="21"/>
<wire x1="3.575" y1="2.3375" x2="3.575" y2="-3.575" width="0.2032" layer="21"/>
<wire x1="3.575" y1="-3.575" x2="-2.3375" y2="-3.575" width="0.2032" layer="21"/>
<wire x1="-2.3375" y1="-3.575" x2="-2.5" y2="-3.4125" width="0.2032" layer="21"/>
<wire x1="-2.5" y1="-3.4125" x2="-3.4125" y2="-2.5" width="0.2032" layer="21" curve="167.429893"/>
<wire x1="-3.4125" y1="-2.5" x2="-3.575" y2="-2.3375" width="0.2032" layer="21"/>
<wire x1="-3.575" y1="-2.3375" x2="-3.575" y2="2.3375" width="0.2032" layer="21"/>
<wire x1="2.3375" y1="3.575" x2="2.5" y2="3.4125" width="0.2032" layer="21"/>
<wire x1="2.5" y1="3.4125" x2="3.4125" y2="2.5" width="0.2032" layer="21" curve="167.429893"/>
<wire x1="3.4125" y1="2.5" x2="3.575" y2="2.3375" width="0.2032" layer="21"/>
<wire x1="-1.725" y1="2.225" x2="-1.0625" y2="2.5625" width="0.2032" layer="21" curve="-255.44999"/>
<wire x1="1.725" y1="-2.225" x2="1.0625" y2="-2.5625" width="0.2032" layer="21" curve="-255.44999"/>
<circle x="0" y="0" radius="2.725" width="0.2032" layer="51"/>
<smd name="1NC" x="-5.2" y="1.15" dx="2.9" dy="1.7" layer="1"/>
<smd name="2+" x="-5.2" y="-1.15" dx="2.9" dy="1.7" layer="1"/>
<smd name="3NC" x="5.2" y="-1.15" dx="2.9" dy="1.7" layer="1" rot="R180"/>
<smd name="4-" x="5.2" y="1.15" dx="2.9" dy="1.7" layer="1" rot="R180"/>
<text x="-3.175" y="3.81" size="1.27" layer="25">&gt;NAME</text>
<text x="-3.175" y="-5.08" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-5.975" y1="0.575" x2="-3.625" y2="1.6" layer="51"/>
<rectangle x1="-5.975" y1="-1.6" x2="-3.625" y2="-0.575" layer="51"/>
<rectangle x1="3.625" y1="-1.6" x2="5.975" y2="-0.575" layer="51" rot="R180"/>
<rectangle x1="3.625" y1="0.575" x2="5.975" y2="1.6" layer="51" rot="R180"/>
<polygon width="0.4064" layer="1">
<vertex x="2.3383" y="1.35"/>
<vertex x="0" y="2.7"/>
<vertex x="-2.3383" y="1.35"/>
<vertex x="-2.3383" y="-1.35"/>
<vertex x="0" y="-2.7"/>
<vertex x="2.3383" y="-1.35"/>
</polygon>
<polygon width="0.4064" layer="29">
<vertex x="2.3383" y="1.35"/>
<vertex x="0" y="2.7"/>
<vertex x="-2.3383" y="1.35"/>
<vertex x="-2.3383" y="-1.35"/>
<vertex x="0" y="-2.7"/>
<vertex x="2.3383" y="-1.35"/>
</polygon>
<polygon width="0.4064" layer="31">
<vertex x="2.3383" y="1.35"/>
<vertex x="0" y="2.7"/>
<vertex x="-2.3383" y="1.35"/>
<vertex x="-2.3383" y="-1.35"/>
<vertex x="0" y="-2.7"/>
<vertex x="2.3383" y="-1.35"/>
</polygon>
</package>
<package name="LUMILED">
<description>&lt;b&gt;Lumileds Lighting. LUXEON®&lt;/b&gt; without cool pad&lt;p&gt;
Source: K2.pdf</description>
<wire x1="-3.575" y1="2.3375" x2="-2.3375" y2="3.575" width="0.2032" layer="21"/>
<wire x1="-2.3375" y1="3.575" x2="2.3375" y2="3.575" width="0.2032" layer="21"/>
<wire x1="3.575" y1="2.3375" x2="3.575" y2="-3.575" width="0.2032" layer="21"/>
<wire x1="3.575" y1="-3.575" x2="-2.3375" y2="-3.575" width="0.2032" layer="21"/>
<wire x1="-2.3375" y1="-3.575" x2="-2.5" y2="-3.4125" width="0.2032" layer="21"/>
<wire x1="-2.5" y1="-3.4125" x2="-3.4125" y2="-2.5" width="0.2032" layer="21" curve="167.429893"/>
<wire x1="-3.4125" y1="-2.5" x2="-3.575" y2="-2.3375" width="0.2032" layer="21"/>
<wire x1="-3.575" y1="-2.3375" x2="-3.575" y2="2.3375" width="0.2032" layer="21"/>
<wire x1="2.3375" y1="3.575" x2="2.5" y2="3.4125" width="0.2032" layer="21"/>
<wire x1="2.5" y1="3.4125" x2="3.4125" y2="2.5" width="0.2032" layer="21" curve="167.429893"/>
<wire x1="3.4125" y1="2.5" x2="3.575" y2="2.3375" width="0.2032" layer="21"/>
<wire x1="-1.725" y1="2.225" x2="-1.0625" y2="2.5625" width="0.2032" layer="21" curve="-255.44999"/>
<wire x1="1.725" y1="-2.225" x2="1.0625" y2="-2.5625" width="0.2032" layer="21" curve="-255.44999"/>
<circle x="0" y="0" radius="2.725" width="0.2032" layer="51"/>
<smd name="1NC" x="-5.2" y="1.15" dx="2.9" dy="1.7" layer="1"/>
<smd name="2+" x="-5.2" y="-1.15" dx="2.9" dy="1.7" layer="1"/>
<smd name="3NC" x="5.2" y="-1.15" dx="2.9" dy="1.7" layer="1" rot="R180"/>
<smd name="4-" x="5.2" y="1.15" dx="2.9" dy="1.7" layer="1" rot="R180"/>
<text x="-3.175" y="3.81" size="1.27" layer="25">&gt;NAME</text>
<text x="-3.175" y="-5.08" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-5.975" y1="0.575" x2="-3.625" y2="1.6" layer="51"/>
<rectangle x1="-5.975" y1="-1.6" x2="-3.625" y2="-0.575" layer="51"/>
<rectangle x1="3.625" y1="-1.6" x2="5.975" y2="-0.575" layer="51" rot="R180"/>
<rectangle x1="3.625" y1="0.575" x2="5.975" y2="1.6" layer="51" rot="R180"/>
<polygon width="0.4064" layer="29">
<vertex x="2.3383" y="1.35"/>
<vertex x="0" y="2.7"/>
<vertex x="-2.3383" y="1.35"/>
<vertex x="-2.3383" y="-1.35"/>
<vertex x="0" y="-2.7"/>
<vertex x="2.3383" y="-1.35"/>
</polygon>
<polygon width="0.4064" layer="31">
<vertex x="2.3383" y="1.35"/>
<vertex x="0" y="2.7"/>
<vertex x="-2.3383" y="1.35"/>
<vertex x="-2.3383" y="-1.35"/>
<vertex x="0" y="-2.7"/>
<vertex x="2.3383" y="-1.35"/>
</polygon>
</package>
<package name="LED10MM">
<description>&lt;B&gt;LED&lt;/B&gt;&lt;p&gt;
10 mm, round</description>
<wire x1="5.08" y1="-2.54" x2="5.08" y2="2.54" width="0.254" layer="21" curve="-306.869898"/>
<wire x1="4.445" y1="0" x2="0" y2="-4.445" width="0.127" layer="21" curve="-90"/>
<wire x1="3.81" y1="0" x2="0" y2="-3.81" width="0.127" layer="21" curve="-90"/>
<wire x1="3.175" y1="0" x2="0" y2="-3.175" width="0.127" layer="21" curve="-90"/>
<wire x1="2.54" y1="0" x2="0" y2="-2.54" width="0.127" layer="21" curve="-90"/>
<wire x1="-4.445" y1="0" x2="0" y2="4.445" width="0.127" layer="21" curve="-90"/>
<wire x1="-3.81" y1="0" x2="0" y2="3.81" width="0.127" layer="21" curve="-90"/>
<wire x1="-3.175" y1="0" x2="0" y2="3.175" width="0.127" layer="21" curve="-90"/>
<wire x1="-2.54" y1="0" x2="0" y2="2.54" width="0.127" layer="21" curve="-90"/>
<wire x1="5.08" y1="2.54" x2="5.08" y2="-2.54" width="0.254" layer="21"/>
<circle x="0" y="0" radius="5.08" width="0.127" layer="21"/>
<pad name="K" x="1.27" y="0" drill="0.8128" diameter="1.6764" shape="square"/>
<pad name="A" x="-1.27" y="0" drill="0.8128" diameter="1.6764" shape="octagon"/>
<text x="6.35" y="1.27" size="1.27" layer="25">&gt;NAME</text>
<text x="6.35" y="-1.27" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="KA-3528ASYC">
<description>&lt;b&gt;SURFACE MOUNT LED LAMP&lt;/b&gt; 3.5x2.8mm&lt;p&gt;
Source: http://www.kingbright.com/manager/upload/pdf/KA-3528ASYC(Ver1189474662.1)</description>
<wire x1="-1.55" y1="1.35" x2="1.55" y2="1.35" width="0.1016" layer="21"/>
<wire x1="1.55" y1="1.35" x2="1.55" y2="-1.35" width="0.1016" layer="51"/>
<wire x1="1.55" y1="-1.35" x2="-1.55" y2="-1.35" width="0.1016" layer="21"/>
<wire x1="-1.55" y1="-1.35" x2="-1.55" y2="1.35" width="0.1016" layer="51"/>
<wire x1="-0.65" y1="0.95" x2="0.65" y2="0.95" width="0.1016" layer="21" curve="-68.40813"/>
<wire x1="0.65" y1="-0.95" x2="-0.65" y2="-0.95" width="0.1016" layer="21" curve="-68.40813"/>
<circle x="0" y="0" radius="1.15" width="0.1016" layer="51"/>
<smd name="A" x="-1.55" y="0" dx="1.5" dy="2.2" layer="1"/>
<smd name="C" x="1.55" y="0" dx="1.5" dy="2.2" layer="1"/>
<text x="-1.905" y="1.905" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.905" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.75" y1="0.6" x2="-1.6" y2="1.1" layer="51"/>
<rectangle x1="-1.75" y1="-1.1" x2="-1.6" y2="-0.6" layer="51"/>
<rectangle x1="1.6" y1="-1.1" x2="1.75" y2="-0.6" layer="51" rot="R180"/>
<rectangle x1="1.6" y1="0.6" x2="1.75" y2="1.1" layer="51" rot="R180"/>
<polygon width="0.1016" layer="51">
<vertex x="1.55" y="-1.35"/>
<vertex x="1.55" y="-0.625"/>
<vertex x="0.825" y="-1.35"/>
</polygon>
<polygon width="0.1016" layer="21">
<vertex x="1.55" y="-1.35"/>
<vertex x="1.55" y="-1.175"/>
<vertex x="1" y="-1.175"/>
<vertex x="0.825" y="-1.35"/>
</polygon>
</package>
<package name="SML0805">
<description>&lt;b&gt;SML0805-2CW-TR (0805 PROFILE)&lt;/b&gt; COOL WHITE&lt;p&gt;
Source: http://www.ledtronics.com/ds/smd-0603/Dstr0093.pdf</description>
<wire x1="-0.95" y1="-0.55" x2="0.95" y2="-0.55" width="0.1016" layer="51"/>
<wire x1="0.95" y1="-0.55" x2="0.95" y2="0.55" width="0.1016" layer="51"/>
<wire x1="0.95" y1="0.55" x2="-0.95" y2="0.55" width="0.1016" layer="51"/>
<wire x1="-0.95" y1="0.55" x2="-0.95" y2="-0.55" width="0.1016" layer="51"/>
<wire x1="-0.175" y1="-0.025" x2="0" y2="0.15" width="0.0634" layer="21"/>
<wire x1="0" y1="0.15" x2="0.15" y2="0" width="0.0634" layer="21"/>
<wire x1="0.15" y1="0" x2="-0.025" y2="-0.175" width="0.0634" layer="21"/>
<wire x1="-0.025" y1="-0.175" x2="-0.175" y2="-0.025" width="0.0634" layer="21"/>
<circle x="-0.275" y="0.4" radius="0.125" width="0" layer="21"/>
<smd name="C" x="-1.05" y="0" dx="1.2" dy="1.2" layer="1"/>
<smd name="A" x="1.05" y="0" dx="1.2" dy="1.2" layer="1"/>
<text x="-1.5" y="1" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.5" y="-2" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="SML1206">
<description>&lt;b&gt;SML10XXKH-TR (HIGH INTENSITY) LED&lt;/b&gt;&lt;p&gt;
&lt;table&gt;
&lt;tr&gt;&lt;td&gt;SML10R3KH-TR&lt;/td&gt;&lt;td&gt;ULTRA RED&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;SML10E3KH-TR&lt;/td&gt;&lt;td&gt;SUPER REDSUPER BLUE&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;SML10O3KH-TR&lt;/td&gt;&lt;td&gt;SUPER ORANGE&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;SML10PY3KH-TR&lt;/td&gt;&lt;td&gt;PURE YELLOW&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;SML10OY3KH-TR&lt;/td&gt;&lt;td&gt;ULTRA YELLOW&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;SML10AG3KH-TR&lt;/td&gt;&lt;td&gt;AQUA GREEN&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;SML10BG3KH-TR&lt;/td&gt;&lt;td&gt;BLUE GREEN&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;SML10PB1KH-TR&lt;/td&gt;&lt;td&gt;SUPER BLUE&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;SML10CW1KH-TR&lt;/td&gt;&lt;td&gt;WHITE&lt;/td&gt;&lt;/tr&gt;
&lt;/table&gt;

Source: http://www.ledtronics.com/ds/smd-1206/dstr0094.PDF</description>
<wire x1="-1.5" y1="0.5" x2="-1.5" y2="-0.5" width="0.2032" layer="51" curve="-180"/>
<wire x1="1.5" y1="-0.5" x2="1.5" y2="0.5" width="0.2032" layer="51" curve="-180"/>
<wire x1="-1.55" y1="0.75" x2="1.55" y2="0.75" width="0.1016" layer="51"/>
<wire x1="1.55" y1="-0.75" x2="-1.55" y2="-0.75" width="0.1016" layer="51"/>
<circle x="-0.725" y="0.525" radius="0.125" width="0" layer="21"/>
<smd name="C" x="-1.75" y="0" dx="1.5" dy="1.5" layer="1"/>
<smd name="A" x="1.75" y="0" dx="1.5" dy="1.5" layer="1"/>
<text x="-1.5" y="1" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.5" y="-2.5" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.6" y1="0.4" x2="-1.15" y2="0.8" layer="51"/>
<rectangle x1="-1.6" y1="-0.8" x2="-1.15" y2="-0.4" layer="51"/>
<rectangle x1="-1.175" y1="-0.6" x2="-1" y2="-0.275" layer="51"/>
<rectangle x1="1.15" y1="-0.8" x2="1.6" y2="-0.4" layer="51" rot="R180"/>
<rectangle x1="1.15" y1="0.4" x2="1.6" y2="0.8" layer="51" rot="R180"/>
<rectangle x1="1" y1="0.275" x2="1.175" y2="0.6" layer="51" rot="R180"/>
<rectangle x1="-0.1" y1="-0.1" x2="0.1" y2="0.1" layer="21"/>
</package>
<package name="SML0603">
<description>&lt;b&gt;SML0603-XXX (HIGH INTENSITY) LED&lt;/b&gt;&lt;p&gt;
&lt;table&gt;
&lt;tr&gt;&lt;td&gt;AG3K&lt;/td&gt;&lt;td&gt;AQUA GREEN&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;B1K&lt;/td&gt;&lt;td&gt;SUPER BLUE&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;R1K&lt;/td&gt;&lt;td&gt;SUPER RED&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;R3K&lt;/td&gt;&lt;td&gt;ULTRA RED&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;O3K&lt;/td&gt;&lt;td&gt;SUPER ORANGE&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;O3KH&lt;/td&gt;&lt;td&gt;SOFT ORANGE&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;Y3KH&lt;/td&gt;&lt;td&gt;SUPER YELLOW&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;Y3K&lt;/td&gt;&lt;td&gt;SUPER YELLOW&lt;/td&gt;&lt;/tr&gt;
&lt;tr&gt;&lt;td&gt;2CW&lt;/td&gt;&lt;td&gt;WHITE&lt;/td&gt;&lt;/tr&gt;
&lt;/table&gt;
Source: http://www.ledtronics.com/ds/smd-0603/Dstr0092.pdf</description>
<wire x1="-0.75" y1="0.35" x2="0.75" y2="0.35" width="0.1016" layer="51"/>
<wire x1="0.75" y1="0.35" x2="0.75" y2="-0.35" width="0.1016" layer="51"/>
<wire x1="0.75" y1="-0.35" x2="-0.75" y2="-0.35" width="0.1016" layer="51"/>
<wire x1="-0.75" y1="-0.35" x2="-0.75" y2="0.35" width="0.1016" layer="51"/>
<wire x1="-0.45" y1="0.3" x2="-0.45" y2="-0.3" width="0.1016" layer="51"/>
<wire x1="0.45" y1="0.3" x2="0.45" y2="-0.3" width="0.1016" layer="51"/>
<wire x1="-0.2" y1="0.35" x2="0.2" y2="0.35" width="0.1016" layer="21"/>
<wire x1="-0.2" y1="-0.35" x2="0.2" y2="-0.35" width="0.1016" layer="21"/>
<smd name="C" x="-0.75" y="0" dx="0.8" dy="0.8" layer="1"/>
<smd name="A" x="0.75" y="0" dx="0.8" dy="0.8" layer="1"/>
<text x="-1" y="1" size="1.27" layer="25">&gt;NAME</text>
<text x="-1" y="-2" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-0.4" y1="0.175" x2="0" y2="0.4" layer="51"/>
<rectangle x1="-0.25" y1="0.175" x2="0" y2="0.4" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="LED">
<wire x1="1.27" y1="0" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="-1.27" y2="0" width="0.254" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="-1.27" y2="-2.54" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="-1.27" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="0" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="-2.032" y1="-0.762" x2="-3.429" y2="-2.159" width="0.1524" layer="94"/>
<wire x1="-1.905" y1="-1.905" x2="-3.302" y2="-3.302" width="0.1524" layer="94"/>
<text x="3.556" y="-4.572" size="1.778" layer="95" rot="R90">&gt;NAME</text>
<text x="5.715" y="-4.572" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="C" x="0" y="-5.08" visible="off" length="short" direction="pas" rot="R90"/>
<pin name="A" x="0" y="2.54" visible="off" length="short" direction="pas" rot="R270"/>
<polygon width="0.1524" layer="94">
<vertex x="-3.429" y="-2.159"/>
<vertex x="-3.048" y="-1.27"/>
<vertex x="-2.54" y="-1.778"/>
</polygon>
<polygon width="0.1524" layer="94">
<vertex x="-3.302" y="-3.302"/>
<vertex x="-2.921" y="-2.413"/>
<vertex x="-2.413" y="-2.921"/>
</polygon>
</symbol>
</symbols>
<devicesets>
<deviceset name="LED" prefix="LED" uservalue="yes">
<description>&lt;b&gt;LED&lt;/b&gt;&lt;p&gt;
&lt;u&gt;OSRAM&lt;/u&gt;:&lt;br&gt;

- &lt;u&gt;CHIPLED&lt;/u&gt;&lt;br&gt;
LG R971, LG N971, LY N971, LG Q971, LY Q971, LO R971, LY R971
LH N974, LH R974&lt;br&gt;
LS Q976, LO Q976, LY Q976&lt;br&gt;
LO Q996&lt;br&gt;

- &lt;u&gt;Hyper CHIPLED&lt;/u&gt;&lt;br&gt;
LW Q18S&lt;br&gt;
LB Q993, LB Q99A, LB R99A&lt;br&gt;

- &lt;u&gt;SideLED&lt;/u&gt;&lt;br&gt;
LS A670, LO A670, LY A670, LG A670, LP A670&lt;br&gt;
LB A673, LV A673, LT A673, LW A673&lt;br&gt;
LH A674&lt;br&gt;
LY A675&lt;br&gt;
LS A676, LA A676, LO A676, LY A676, LW A676&lt;br&gt;
LS A679, LY A679, LG A679&lt;br&gt;

-  &lt;u&gt;Hyper Micro SIDELED®&lt;/u&gt;&lt;br&gt;
LS Y876, LA Y876, LO Y876, LY Y876&lt;br&gt;
LT Y87S&lt;br&gt;

- &lt;u&gt;SmartLED&lt;/u&gt;&lt;br&gt;
LW L88C, LW L88S&lt;br&gt;
LB L89C, LB L89S, LG L890&lt;br&gt;
LS L89K, LO L89K, LY L89K&lt;br&gt;
LS L896, LA L896, LO L896, LY L896&lt;br&gt;

- &lt;u&gt;TOPLED&lt;/u&gt;&lt;br&gt;
LS T670, LO T670, LY T670, LG T670, LP T670&lt;br&gt;
LSG T670, LSP T670, LSY T670, LOP T670, LYG T670&lt;br&gt;
LG T671, LOG T671, LSG T671&lt;br&gt;
LB T673, LV T673, LT T673, LW T673&lt;br&gt;
LH T674&lt;br&gt;
LS T676, LA T676, LO T676, LY T676, LB T676, LH T676, LSB T676, LW T676&lt;br&gt;
LB T67C, LV T67C, LT T67C, LS T67K, LO T67K, LY T67K, LW E67C&lt;br&gt;
LS E67B, LA E67B, LO E67B, LY E67B, LB E67C, LV E67C, LT E67C&lt;br&gt;
LW T67C&lt;br&gt;
LS T679, LY T679, LG T679&lt;br&gt;
LS T770, LO T770, LY T770, LG T770, LP T770&lt;br&gt;
LB T773, LV T773, LT T773, LW T773&lt;br&gt;
LH T774&lt;br&gt;
LS E675, LA E675, LY E675, LS T675&lt;br&gt;
LS T776, LA T776, LO T776, LY T776, LB T776&lt;br&gt;
LHGB T686&lt;br&gt;
LT T68C, LB T68C&lt;br&gt;

- &lt;u&gt;Hyper Mini TOPLED®&lt;/u&gt;&lt;br&gt;
LB M676&lt;br&gt;

- &lt;u&gt;Mini TOPLED Santana®&lt;/u&gt;&lt;br&gt;
LG M470&lt;br&gt;
LS M47K, LO M47K, LY M47K
&lt;p&gt;
Source: http://www.osram.convergy.de&lt;p&gt;

&lt;u&gt;LUXEON:&lt;/u&gt;&lt;br&gt;
- &lt;u&gt;LUMILED®&lt;/u&gt;&lt;br&gt;
LXK2-PW12-R00, LXK2-PW12-S00, LXK2-PW14-U00, LXK2-PW14-V00&lt;br&gt;
LXK2-PM12-R00, LXK2-PM12-S00, LXK2-PM14-U00&lt;br&gt;
LXK2-PE12-Q00, LXK2-PE12-R00, LXK2-PE12-S00, LXK2-PE14-T00, LXK2-PE14-U00&lt;br&gt;
LXK2-PB12-K00, LXK2-PB12-L00, LXK2-PB12-M00, LXK2-PB14-N00, LXK2-PB14-P00, LXK2-PB14-Q00&lt;br&gt;
LXK2-PR12-L00, LXK2-PR12-M00, LXK2-PR14-Q00, LXK2-PR14-R00&lt;br&gt;
LXK2-PD12-Q00, LXK2-PD12-R00, LXK2-PD12-S00&lt;br&gt;
LXK2-PH12-R00, LXK2-PH12-S00&lt;br&gt;
LXK2-PL12-P00, LXK2-PL12-Q00, LXK2-PL12-R00
&lt;p&gt;
Source: www.luxeon.com&lt;p&gt;

&lt;u&gt;KINGBRIGHT:&lt;/U&gt;&lt;p&gt;
KA-3528ASYC&lt;br&gt;
Source: www.kingbright.com</description>
<gates>
<gate name="G$1" symbol="LED" x="0" y="0"/>
</gates>
<devices>
<device name="SMT1206" package="1206">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="LD260" package="LD260">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SQR2X5" package="LED2X5">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="3MM" package="LED3MM">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="5MM" package="LED5MM">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="LSU260" package="LSU260">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="LZR181" package="LZR181">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="B152" package="Q62902-B152">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="B153" package="Q62902-B153">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="B155" package="Q62902-B155">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="B156" package="Q62902-B156">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SFH480" package="SFH480">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SFH482" package="SFH482">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SQR5.7X3.2" package="U57X32">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="IRL80A" package="IRL80A">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="P-LCC-2" package="P-LCC-2">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="MINI-TOP" package="OSRAM-MINI-TOP-LED">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SIDELED" package="OSRAM-SIDELED">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMART-LED" package="SMART-LED">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="B"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="P-LCC-2-BACK" package="P-LCC-2-TOPLED-RG">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="MICRO-SIDELED" package="MICRO-SIDELED">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="P-LCC-4" package="P-LCC-4">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C@4"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="CHIP-LED0603" package="CHIP-LED0603">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="CHIP-LED0805" package="CHIP-LED0805">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="TOPLED-SANTANA" package="MINI-TOPLED-SANTANA">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="CHIPLED_0805" package="CHIPLED_0805">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="CHIPLED_1206" package="CHIPLED_1206">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="CHIPLED_0603" package="CHIPLED_0603">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="CHIPLED-0603-TTW" package="CHIPLED-0603-TTW">
<connects>
<connect gate="G$1" pin="A" pad="A@1"/>
<connect gate="G$1" pin="C" pad="C@1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="" package="SMARTLED-TTW">
<connects>
<connect gate="G$1" pin="A" pad="A@1"/>
<connect gate="G$1" pin="C" pad="C@1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="-LUMILED+" package="LUMILED+">
<connects>
<connect gate="G$1" pin="A" pad="2+"/>
<connect gate="G$1" pin="C" pad="4-"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="-LUMILED" package="LUMILED">
<connects>
<connect gate="G$1" pin="A" pad="2+"/>
<connect gate="G$1" pin="C" pad="4-"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="10MM" package="LED10MM">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="K"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="KA-3528ASYC" package="KA-3528ASYC">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SML0805" package="SML0805">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SML1206" package="SML1206">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SML0603" package="SML0603">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="switch-misc">
<description>&lt;b&gt;Switches&lt;/b&gt;&lt;p&gt;
Marquardt, RAFI, ITT, and others&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="9450-1">
<description>&lt;b&gt;MARQUARDT SWITCH&lt;/b&gt;</description>
<wire x1="-3.556" y1="-2.286" x2="3.556" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="3.556" y1="2.286" x2="3.556" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="3.556" y1="2.286" x2="-3.556" y2="2.286" width="0.1524" layer="21"/>
<wire x1="-3.556" y1="-2.286" x2="-3.556" y2="2.286" width="0.1524" layer="21"/>
<wire x1="-0.762" y1="-1.905" x2="0.762" y2="-1.905" width="0.1524" layer="51"/>
<wire x1="3.175" y1="1.905" x2="3.175" y2="-0.381" width="0.1524" layer="21"/>
<wire x1="3.175" y1="1.905" x2="-3.175" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-0.381" x2="-3.175" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-1.9889" y1="-0.4165" x2="1.9889" y2="-0.4165" width="0.1524" layer="21" curve="-203.655043"/>
<wire x1="-0.8312" y1="-1.8542" x2="0.8312" y2="-1.8542" width="0.1524" layer="51" curve="48.291439"/>
<wire x1="-1.9889" y1="-0.4165" x2="-1.5265" y2="-1.3412" width="0.1524" layer="51" curve="29.474771"/>
<wire x1="-1.5265" y1="-1.3412" x2="-0.8312" y2="-1.8542" width="0.1524" layer="21" curve="24.55173"/>
<wire x1="1.5265" y1="-1.3412" x2="1.9889" y2="-0.4165" width="0.1524" layer="51" curve="29.474771"/>
<wire x1="0.8312" y1="-1.8542" x2="1.5265" y2="-1.3412" width="0.1524" layer="21" curve="24.55173"/>
<wire x1="0.762" y1="-1.905" x2="1.778" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-0.762" y1="-1.905" x2="-1.778" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-1.778" y1="-1.905" x2="-3.175" y2="-1.905" width="0.1524" layer="51"/>
<wire x1="-3.175" y1="-1.905" x2="-3.175" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="1.778" y1="-1.905" x2="3.175" y2="-1.905" width="0.1524" layer="51"/>
<wire x1="3.175" y1="-1.905" x2="3.175" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="1.5265" y1="-1.3412" x2="1.9889" y2="-0.4165" width="0.1524" layer="51" curve="29.474771"/>
<wire x1="-1.0155" y1="1.3018" x2="-1.0155" y2="-1.3018" width="0.1524" layer="21" curve="104.086284"/>
<wire x1="1.0155" y1="-1.3018" x2="1.0155" y2="1.3018" width="0.1524" layer="21" curve="104.086284"/>
<wire x1="-0.8312" y1="-1.8542" x2="0.8312" y2="-1.8542" width="0.1524" layer="51" curve="48.291439"/>
<wire x1="-1.0155" y1="-1.3018" x2="1.0155" y2="-1.3018" width="0.1524" layer="51" curve="75.913716"/>
<wire x1="-1.9889" y1="-0.4165" x2="-1.5265" y2="-1.3412" width="0.1524" layer="51" curve="29.474771"/>
<wire x1="-1.5265" y1="-1.3412" x2="-0.8312" y2="-1.8542" width="0.1524" layer="21" curve="24.55173"/>
<wire x1="0.8312" y1="-1.8542" x2="1.5265" y2="-1.3412" width="0.1524" layer="21" curve="24.55173"/>
<wire x1="-1.0155" y1="1.3018" x2="1.0155" y2="1.3018" width="0.1524" layer="51" curve="-75.913716"/>
<pad name="2" x="0" y="-1.27" drill="0.8128" shape="long" rot="R90"/>
<pad name="1" x="-2.54" y="-1.27" drill="0.8128" shape="long" rot="R90"/>
<pad name="3" x="2.54" y="-1.27" drill="0.8128" shape="long" rot="R90"/>
<text x="-3.302" y="2.794" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.302" y="-4.064" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<hole x="2.54" y="1.27" drill="0.8128"/>
<hole x="-2.54" y="1.27" drill="0.8128"/>
</package>
</packages>
<symbols>
<symbol name="TSU">
<wire x1="0" y1="-3.175" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="-1.905" y2="3.175" width="0.254" layer="94"/>
<wire x1="1.27" y1="2.54" x2="2.54" y2="2.54" width="0.254" layer="94"/>
<wire x1="2.54" y1="2.54" x2="2.54" y2="3.175" width="0.254" layer="94"/>
<wire x1="-2.54" y1="2.54" x2="-1.27" y2="2.54" width="0.254" layer="94"/>
<wire x1="-2.54" y1="2.54" x2="-2.54" y2="3.175" width="0.254" layer="94"/>
<wire x1="-0.889" y1="0" x2="-1.27" y2="0" width="0.1524" layer="94"/>
<wire x1="-1.905" y1="0" x2="-2.54" y2="0" width="0.1524" layer="94"/>
<wire x1="-3.175" y1="0" x2="-3.81" y2="0" width="0.1524" layer="94"/>
<wire x1="-6.35" y1="1.905" x2="-5.08" y2="1.905" width="0.254" layer="94"/>
<wire x1="-6.35" y1="1.905" x2="-6.35" y2="0" width="0.254" layer="94"/>
<wire x1="-6.35" y1="-1.905" x2="-5.08" y2="-1.905" width="0.254" layer="94"/>
<wire x1="-6.35" y1="0" x2="-4.445" y2="0" width="0.1524" layer="94"/>
<wire x1="-6.35" y1="0" x2="-6.35" y2="-1.905" width="0.254" layer="94"/>
<text x="-8.255" y="-1.905" size="1.778" layer="95" rot="R90">&gt;NAME</text>
<text x="-5.715" y="2.54" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="P" x="0" y="-5.08" visible="pad" length="short" direction="pas" rot="R90"/>
<pin name="S" x="2.54" y="5.08" visible="pad" length="short" direction="pas" rot="R270"/>
<pin name="O" x="-2.54" y="5.08" visible="pad" length="short" direction="pas" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="9450-1" prefix="S">
<description>&lt;b&gt;MARQUARDT SWITCH&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="TSU" x="0" y="0"/>
</gates>
<devices>
<device name="" package="9450-1">
<connects>
<connect gate="1" pin="O" pad="3"/>
<connect gate="1" pin="P" pad="2"/>
<connect gate="1" pin="S" pad="1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="resistor">
<description>&lt;b&gt;Resistors, Capacitors, Inductors&lt;/b&gt;&lt;p&gt;
Based on the previous libraries:
&lt;ul&gt;
&lt;li&gt;r.lbr
&lt;li&gt;cap.lbr 
&lt;li&gt;cap-fe.lbr
&lt;li&gt;captant.lbr
&lt;li&gt;polcap.lbr
&lt;li&gt;ipc-smd.lbr
&lt;/ul&gt;
All SMD packages are defined according to the IPC specifications and  CECC&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;&lt;p&gt;
&lt;p&gt;
for Electrolyt Capacitors see also :&lt;p&gt;
www.bccomponents.com &lt;p&gt;
www.panasonic.com&lt;p&gt;
www.kemet.com&lt;p&gt;
&lt;p&gt;
for trimmer refence see : &lt;u&gt;www.electrospec-inc.com/cross_references/trimpotcrossref.asp&lt;/u&gt;&lt;p&gt;

&lt;map name="nav_main"&gt;
&lt;area shape="rect" coords="0,1,140,23" href="../military_specs.asp" title=""&gt;
&lt;area shape="rect" coords="0,24,140,51" href="../about.asp" title=""&gt;
&lt;area shape="rect" coords="1,52,140,77" href="../rfq.asp" title=""&gt;
&lt;area shape="rect" coords="0,78,139,103" href="../products.asp" title=""&gt;
&lt;area shape="rect" coords="1,102,138,128" href="../excess_inventory.asp" title=""&gt;
&lt;area shape="rect" coords="1,129,138,150" href="../edge.asp" title=""&gt;
&lt;area shape="rect" coords="1,151,139,178" href="../industry_links.asp" title=""&gt;
&lt;area shape="rect" coords="0,179,139,201" href="../comments.asp" title=""&gt;
&lt;area shape="rect" coords="1,203,138,231" href="../directory.asp" title=""&gt;
&lt;area shape="default" nohref&gt;
&lt;/map&gt;

&lt;html&gt;

&lt;title&gt;&lt;/title&gt;

 &lt;LINK REL="StyleSheet" TYPE="text/css" HREF="style-sheet.css"&gt;

&lt;body bgcolor="#ffffff" text="#000000" marginwidth="0" marginheight="0" topmargin="0" leftmargin="0"&gt;
&lt;table border=0 cellspacing=0 cellpadding=0 width="100%" cellpaddding=0 height="55%"&gt;
&lt;tr valign="top"&gt;

&lt;/td&gt;
&lt;! &lt;td width="10"&gt;&amp;nbsp;&lt;/td&gt;
&lt;td width="90%"&gt;

&lt;b&gt;&lt;font color="#0000FF" size="4"&gt;TRIM-POT CROSS REFERENCE&lt;/font&gt;&lt;/b&gt;
&lt;P&gt;
&lt;TABLE BORDER=0 CELLSPACING=1 CELLPADDING=2&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=8&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;RECTANGULAR MULTI-TURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;BOURNS&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;BI&amp;nbsp;TECH&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;DALE-VISHAY&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;PHILIPS/MEPCO&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;MURATA&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;PANASONIC&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;SPECTROL&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;MILSPEC&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;&lt;TD&gt;&amp;nbsp;&lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3 &gt;
      3005P&lt;BR&gt;
      3006P&lt;BR&gt;
      3006W&lt;BR&gt;
      3006Y&lt;BR&gt;
      3009P&lt;BR&gt;
      3009W&lt;BR&gt;
      3009Y&lt;BR&gt;
      3057J&lt;BR&gt;
      3057L&lt;BR&gt;
      3057P&lt;BR&gt;
      3057Y&lt;BR&gt;
      3059J&lt;BR&gt;
      3059L&lt;BR&gt;
      3059P&lt;BR&gt;
      3059Y&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      89P&lt;BR&gt;
      89W&lt;BR&gt;
      89X&lt;BR&gt;
      89PH&lt;BR&gt;
      76P&lt;BR&gt;
      89XH&lt;BR&gt;
      78SLT&lt;BR&gt;
      78L&amp;nbsp;ALT&lt;BR&gt;
      56P&amp;nbsp;ALT&lt;BR&gt;
      78P&amp;nbsp;ALT&lt;BR&gt;
      T8S&lt;BR&gt;
      78L&lt;BR&gt;
      56P&lt;BR&gt;
      78P&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      T18/784&lt;BR&gt;
      783&lt;BR&gt;
      781&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      2199&lt;BR&gt;
      1697/1897&lt;BR&gt;
      1680/1880&lt;BR&gt;
      2187&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      8035EKP/CT20/RJ-20P&lt;BR&gt;
      -&lt;BR&gt;
      RJ-20X&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      1211L&lt;BR&gt;
      8012EKQ&amp;nbsp;ALT&lt;BR&gt;
      8012EKR&amp;nbsp;ALT&lt;BR&gt;
      1211P&lt;BR&gt;
      8012EKJ&lt;BR&gt;
      8012EKL&lt;BR&gt;
      8012EKQ&lt;BR&gt;
      8012EKR&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      2101P&lt;BR&gt;
      2101W&lt;BR&gt;
      2101Y&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      2102L&lt;BR&gt;
      2102S&lt;BR&gt;
      2102Y&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      EVMCOG&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      43P&lt;BR&gt;
      43W&lt;BR&gt;
      43Y&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      40L&lt;BR&gt;
      40P&lt;BR&gt;
      40Y&lt;BR&gt;
      70Y-T602&lt;BR&gt;
      70L&lt;BR&gt;
      70P&lt;BR&gt;
      70Y&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      RT/RTR12&lt;BR&gt;
      RT/RTR12&lt;BR&gt;
      RT/RTR12&lt;BR&gt;
      -&lt;BR&gt;
      RJ/RJR12&lt;BR&gt;
      RJ/RJR12&lt;BR&gt;
      RJ/RJR12&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=8&gt;&amp;nbsp;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=8&gt;
      &lt;FONT SIZE=4 FACE=ARIAL&gt;&lt;B&gt;SQUARE MULTI-TURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
   &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BOURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BI&amp;nbsp;TECH&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;DALE-VISHAY&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PHILIPS/MEPCO&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;MURATA&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PANASONIC&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;SPECTROL&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;MILSPEC&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      3250L&lt;BR&gt;
      3250P&lt;BR&gt;
      3250W&lt;BR&gt;
      3250X&lt;BR&gt;
      3252P&lt;BR&gt;
      3252W&lt;BR&gt;
      3252X&lt;BR&gt;
      3260P&lt;BR&gt;
      3260W&lt;BR&gt;
      3260X&lt;BR&gt;
      3262P&lt;BR&gt;
      3262W&lt;BR&gt;
      3262X&lt;BR&gt;
      3266P&lt;BR&gt;
      3266W&lt;BR&gt;
      3266X&lt;BR&gt;
      3290H&lt;BR&gt;
      3290P&lt;BR&gt;
      3290W&lt;BR&gt;
      3292P&lt;BR&gt;
      3292W&lt;BR&gt;
      3292X&lt;BR&gt;
      3296P&lt;BR&gt;
      3296W&lt;BR&gt;
      3296X&lt;BR&gt;
      3296Y&lt;BR&gt;
      3296Z&lt;BR&gt;
      3299P&lt;BR&gt;
      3299W&lt;BR&gt;
      3299X&lt;BR&gt;
      3299Y&lt;BR&gt;
      3299Z&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      66P&amp;nbsp;ALT&lt;BR&gt;
      66W&amp;nbsp;ALT&lt;BR&gt;
      66X&amp;nbsp;ALT&lt;BR&gt;
      66P&amp;nbsp;ALT&lt;BR&gt;
      66W&amp;nbsp;ALT&lt;BR&gt;
      66X&amp;nbsp;ALT&lt;BR&gt;
      -&lt;BR&gt;
      64W&amp;nbsp;ALT&lt;BR&gt;
      -&lt;BR&gt;
      64P&amp;nbsp;ALT&lt;BR&gt;
      64W&amp;nbsp;ALT&lt;BR&gt;
      64X&amp;nbsp;ALT&lt;BR&gt;
      64P&lt;BR&gt;
      64W&lt;BR&gt;
      64X&lt;BR&gt;
      66X&amp;nbsp;ALT&lt;BR&gt;
      66P&amp;nbsp;ALT&lt;BR&gt;
      66W&amp;nbsp;ALT&lt;BR&gt;
      66P&lt;BR&gt;
      66W&lt;BR&gt;
      66X&lt;BR&gt;
      67P&lt;BR&gt;
      67W&lt;BR&gt;
      67X&lt;BR&gt;
      67Y&lt;BR&gt;
      67Z&lt;BR&gt;
      68P&lt;BR&gt;
      68W&lt;BR&gt;
      68X&lt;BR&gt;
      67Y&amp;nbsp;ALT&lt;BR&gt;
      67Z&amp;nbsp;ALT&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      5050&lt;BR&gt;
      5091&lt;BR&gt;
      5080&lt;BR&gt;
      5087&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      T63YB&lt;BR&gt;
      T63XB&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      5887&lt;BR&gt;
      5891&lt;BR&gt;
      5880&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      T93Z&lt;BR&gt;
      T93YA&lt;BR&gt;
      T93XA&lt;BR&gt;
      T93YB&lt;BR&gt;
      T93XB&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      8026EKP&lt;BR&gt;
      8026EKW&lt;BR&gt;
      8026EKM&lt;BR&gt;
      8026EKP&lt;BR&gt;
      8026EKB&lt;BR&gt;
      8026EKM&lt;BR&gt;
      1309X&lt;BR&gt;
      1309P&lt;BR&gt;
      1309W&lt;BR&gt;
      8024EKP&lt;BR&gt;
      8024EKW&lt;BR&gt;
      8024EKN&lt;BR&gt;
      RJ-9P/CT9P&lt;BR&gt;
      RJ-9W&lt;BR&gt;
      RJ-9X&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      3103P&lt;BR&gt;
      3103Y&lt;BR&gt;
      3103Z&lt;BR&gt;
      3103P&lt;BR&gt;
      3103Y&lt;BR&gt;
      3103Z&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      3105P/3106P&lt;BR&gt;
      3105W/3106W&lt;BR&gt;
      3105X/3106X&lt;BR&gt;
      3105Y/3106Y&lt;BR&gt;
      3105Z/3105Z&lt;BR&gt;
      3102P&lt;BR&gt;
      3102W&lt;BR&gt;
      3102X&lt;BR&gt;
      3102Y&lt;BR&gt;
      3102Z&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      EVMCBG&lt;BR&gt;
      EVMCCG&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      55-1-X&lt;BR&gt;
      55-4-X&lt;BR&gt;
      55-3-X&lt;BR&gt;
      55-2-X&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      50-2-X&lt;BR&gt;
      50-4-X&lt;BR&gt;
      50-3-X&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      64P&lt;BR&gt;
      64W&lt;BR&gt;
      64X&lt;BR&gt;
      64Y&lt;BR&gt;
      64Z&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      RT/RTR22&lt;BR&gt;
      RT/RTR22&lt;BR&gt;
      RT/RTR22&lt;BR&gt;
      RT/RTR22&lt;BR&gt;
      RJ/RJR22&lt;BR&gt;
      RJ/RJR22&lt;BR&gt;
      RJ/RJR22&lt;BR&gt;
      RT/RTR26&lt;BR&gt;
      RT/RTR26&lt;BR&gt;
      RT/RTR26&lt;BR&gt;
      RJ/RJR26&lt;BR&gt;
      RJ/RJR26&lt;BR&gt;
      RJ/RJR26&lt;BR&gt;
      RJ/RJR26&lt;BR&gt;
      RJ/RJR26&lt;BR&gt;
      RJ/RJR26&lt;BR&gt;
      RT/RTR24&lt;BR&gt;
      RT/RTR24&lt;BR&gt;
      RT/RTR24&lt;BR&gt;
      RJ/RJR24&lt;BR&gt;
      RJ/RJR24&lt;BR&gt;
      RJ/RJR24&lt;BR&gt;
      RJ/RJR24&lt;BR&gt;
      RJ/RJR24&lt;BR&gt;
      RJ/RJR24&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=8&gt;&amp;nbsp;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=8&gt;
      &lt;FONT SIZE=4 FACE=ARIAL&gt;&lt;B&gt;SINGLE TURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BOURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BI&amp;nbsp;TECH&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;DALE-VISHAY&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PHILIPS/MEPCO&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;MURATA&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PANASONIC&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;SPECTROL&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;MILSPEC&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      3323P&lt;BR&gt;
      3323S&lt;BR&gt;
      3323W&lt;BR&gt;
      3329H&lt;BR&gt;
      3329P&lt;BR&gt;
      3329W&lt;BR&gt;
      3339H&lt;BR&gt;
      3339P&lt;BR&gt;
      3339W&lt;BR&gt;
      3352E&lt;BR&gt;
      3352H&lt;BR&gt;
      3352K&lt;BR&gt;
      3352P&lt;BR&gt;
      3352T&lt;BR&gt;
      3352V&lt;BR&gt;
      3352W&lt;BR&gt;
      3362H&lt;BR&gt;
      3362M&lt;BR&gt;
      3362P&lt;BR&gt;
      3362R&lt;BR&gt;
      3362S&lt;BR&gt;
      3362U&lt;BR&gt;
      3362W&lt;BR&gt;
      3362X&lt;BR&gt;
      3386B&lt;BR&gt;
      3386C&lt;BR&gt;
      3386F&lt;BR&gt;
      3386H&lt;BR&gt;
      3386K&lt;BR&gt;
      3386M&lt;BR&gt;
      3386P&lt;BR&gt;
      3386S&lt;BR&gt;
      3386W&lt;BR&gt;
      3386X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      25P&lt;BR&gt;
      25S&lt;BR&gt;
      25RX&lt;BR&gt;
      82P&lt;BR&gt;
      82M&lt;BR&gt;
      82PA&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      91E&lt;BR&gt;
      91X&lt;BR&gt;
      91T&lt;BR&gt;
      91B&lt;BR&gt;
      91A&lt;BR&gt;
      91V&lt;BR&gt;
      91W&lt;BR&gt;
      25W&lt;BR&gt;
      25V&lt;BR&gt;
      25P&lt;BR&gt;
      -&lt;BR&gt;
      25S&lt;BR&gt;
      25U&lt;BR&gt;
      25RX&lt;BR&gt;
      25X&lt;BR&gt;
      72XW&lt;BR&gt;
      72XL&lt;BR&gt;
      72PM&lt;BR&gt;
      72RX&lt;BR&gt;
      -&lt;BR&gt;
      72PX&lt;BR&gt;
      72P&lt;BR&gt;
      72RXW&lt;BR&gt;
      72RXL&lt;BR&gt;
      72X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      T7YB&lt;BR&gt;
      T7YA&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      TXD&lt;BR&gt;
      TYA&lt;BR&gt;
      TYP&lt;BR&gt;
      -&lt;BR&gt;
      TYD&lt;BR&gt;
      TX&lt;BR&gt;
      -&lt;BR&gt;
      150SX&lt;BR&gt;
      100SX&lt;BR&gt;
      102T&lt;BR&gt;
      101S&lt;BR&gt;
      190T&lt;BR&gt;
      150TX&lt;BR&gt;
      101&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      101SX&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      ET6P&lt;BR&gt;
      ET6S&lt;BR&gt;
      ET6X&lt;BR&gt;
      RJ-6W/8014EMW&lt;BR&gt;
      RJ-6P/8014EMP&lt;BR&gt;
      RJ-6X/8014EMX&lt;BR&gt;
      TM7W&lt;BR&gt;
      TM7P&lt;BR&gt;
      TM7X&lt;BR&gt;
      -&lt;BR&gt;
      8017SMS&lt;BR&gt;
      -&lt;BR&gt;
      8017SMB&lt;BR&gt;
      8017SMA&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      CT-6W&lt;BR&gt;
      CT-6H&lt;BR&gt;
      CT-6P&lt;BR&gt;
      CT-6R&lt;BR&gt;
      -&lt;BR&gt;
      CT-6V&lt;BR&gt;
      CT-6X&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      8038EKV&lt;BR&gt;
      -&lt;BR&gt;
      8038EKX&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      8038EKP&lt;BR&gt;
      8038EKZ&lt;BR&gt;
      8038EKW&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      3321H&lt;BR&gt;
      3321P&lt;BR&gt;
      3321N&lt;BR&gt;
      1102H&lt;BR&gt;
      1102P&lt;BR&gt;
      1102T&lt;BR&gt;
      RVA0911V304A&lt;BR&gt;
      -&lt;BR&gt;
      RVA0911H413A&lt;BR&gt;
      RVG0707V100A&lt;BR&gt;
      RVA0607V(H)306A&lt;BR&gt;
      RVA1214H213A&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      3104B&lt;BR&gt;
      3104C&lt;BR&gt;
      3104F&lt;BR&gt;
      3104H&lt;BR&gt;
      -&lt;BR&gt;
      3104M&lt;BR&gt;
      3104P&lt;BR&gt;
      3104S&lt;BR&gt;
      3104W&lt;BR&gt;
      3104X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      EVMQ0G&lt;BR&gt;
      EVMQIG&lt;BR&gt;
      EVMQ3G&lt;BR&gt;
      EVMS0G&lt;BR&gt;
      EVMQ0G&lt;BR&gt;
      EVMG0G&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      EVMK4GA00B&lt;BR&gt;
      EVM30GA00B&lt;BR&gt;
      EVMK0GA00B&lt;BR&gt;
      EVM38GA00B&lt;BR&gt;
      EVMB6&lt;BR&gt;
      EVLQ0&lt;BR&gt;
      -&lt;BR&gt;
      EVMMSG&lt;BR&gt;
      EVMMBG&lt;BR&gt;
      EVMMAG&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      EVMMCS&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      EVMM1&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      EVMM0&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      EVMM3&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      62-3-1&lt;BR&gt;
      62-1-2&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      67R&lt;BR&gt;
      -&lt;BR&gt;
      67P&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      67X&lt;BR&gt;
      63V&lt;BR&gt;
      63S&lt;BR&gt;
      63M&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      63H&lt;BR&gt;
      63P&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      63X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      RJ/RJR50&lt;BR&gt;
      RJ/RJR50&lt;BR&gt;
      RJ/RJR50&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
&lt;/TABLE&gt;
&lt;P&gt;&amp;nbsp;&lt;P&gt;
&lt;TABLE BORDER=0 CELLSPACING=1 CELLPADDING=3&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=7&gt;
      &lt;FONT color="#0000FF" SIZE=4 FACE=ARIAL&gt;&lt;B&gt;SMD TRIM-POT CROSS REFERENCE&lt;/B&gt;&lt;/FONT&gt;
      &lt;P&gt;
      &lt;FONT SIZE=4 FACE=ARIAL&gt;&lt;B&gt;MULTI-TURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BOURNS&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BI&amp;nbsp;TECH&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;DALE-VISHAY&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PHILIPS/MEPCO&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PANASONIC&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;TOCOS&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;AUX/KYOCERA&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      3224G&lt;BR&gt;
      3224J&lt;BR&gt;
      3224W&lt;BR&gt;
      3269P&lt;BR&gt;
      3269W&lt;BR&gt;
      3269X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      44G&lt;BR&gt;
      44J&lt;BR&gt;
      44W&lt;BR&gt;
      84P&lt;BR&gt;
      84W&lt;BR&gt;
      84X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      ST63Z&lt;BR&gt;
      ST63Y&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      ST5P&lt;BR&gt;
      ST5W&lt;BR&gt;
      ST5X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=7&gt;&amp;nbsp;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=7&gt;
      &lt;FONT SIZE=4 FACE=ARIAL&gt;&lt;B&gt;SINGLE TURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BOURNS&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BI&amp;nbsp;TECH&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;DALE-VISHAY&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PHILIPS/MEPCO&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PANASONIC&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;TOCOS&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;AUX/KYOCERA&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      3314G&lt;BR&gt;
      3314J&lt;BR&gt;
      3364A/B&lt;BR&gt;
      3364C/D&lt;BR&gt;
      3364W/X&lt;BR&gt;
      3313G&lt;BR&gt;
      3313J&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      23B&lt;BR&gt;
      23A&lt;BR&gt;
      21X&lt;BR&gt;
      21W&lt;BR&gt;
      -&lt;BR&gt;
      22B&lt;BR&gt;
      22A&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      ST5YL/ST53YL&lt;BR&gt;
      ST5YJ/5T53YJ&lt;BR&gt;
      ST-23A&lt;BR&gt;
      ST-22B&lt;BR&gt;
      ST-22&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      ST-4B&lt;BR&gt;
      ST-4A&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      ST-3B&lt;BR&gt;
      ST-3A&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      EVM-6YS&lt;BR&gt;
      EVM-1E&lt;BR&gt;
      EVM-1G&lt;BR&gt;
      EVM-1D&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      G4B&lt;BR&gt;
      G4A&lt;BR&gt;
      TR04-3S1&lt;BR&gt;
      TRG04-2S1&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      DVR-43A&lt;BR&gt;
      CVR-42C&lt;BR&gt;
      CVR-42A/C&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
&lt;/TABLE&gt;
&lt;P&gt;
&lt;FONT SIZE=4 FACE=ARIAL&gt;&lt;B&gt;ALT =&amp;nbsp;ALTERNATE&lt;/B&gt;&lt;/FONT&gt;
&lt;P&gt;

&amp;nbsp;
&lt;P&gt;
&lt;/td&gt;
&lt;/tr&gt;
&lt;/table&gt;
&lt;/BODY&gt;&lt;/HTML&gt;</description>
<packages>
<package name="R0402">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-0.245" y1="0.224" x2="0.245" y2="0.224" width="0.1524" layer="51"/>
<wire x1="0.245" y1="-0.224" x2="-0.245" y2="-0.224" width="0.1524" layer="51"/>
<wire x1="-1.473" y1="0.483" x2="1.473" y2="0.483" width="0.0508" layer="39"/>
<wire x1="1.473" y1="0.483" x2="1.473" y2="-0.483" width="0.0508" layer="39"/>
<wire x1="1.473" y1="-0.483" x2="-1.473" y2="-0.483" width="0.0508" layer="39"/>
<wire x1="-1.473" y1="-0.483" x2="-1.473" y2="0.483" width="0.0508" layer="39"/>
<smd name="1" x="-0.65" y="0" dx="0.7" dy="0.9" layer="1"/>
<smd name="2" x="0.65" y="0" dx="0.7" dy="0.9" layer="1"/>
<text x="-0.635" y="0.762" size="1.27" layer="25">&gt;NAME</text>
<text x="-0.635" y="-2.032" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-0.554" y1="-0.3048" x2="-0.254" y2="0.2951" layer="51"/>
<rectangle x1="0.2588" y1="-0.3048" x2="0.5588" y2="0.2951" layer="51"/>
<rectangle x1="-0.1999" y1="-0.4001" x2="0.1999" y2="0.4001" layer="35"/>
</package>
<package name="R0603">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-0.432" y1="-0.356" x2="0.432" y2="-0.356" width="0.1524" layer="51"/>
<wire x1="0.432" y1="0.356" x2="-0.432" y2="0.356" width="0.1524" layer="51"/>
<wire x1="-1.473" y1="0.983" x2="1.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.473" y1="0.983" x2="1.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="1.473" y1="-0.983" x2="-1.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.473" y1="-0.983" x2="-1.473" y2="0.983" width="0.0508" layer="39"/>
<smd name="1" x="-0.85" y="0" dx="1" dy="1.1" layer="1"/>
<smd name="2" x="0.85" y="0" dx="1" dy="1.1" layer="1"/>
<text x="-0.889" y="0.889" size="1.27" layer="25">&gt;NAME</text>
<text x="-0.889" y="-2.032" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="0.4318" y1="-0.4318" x2="0.8382" y2="0.4318" layer="51"/>
<rectangle x1="-0.8382" y1="-0.4318" x2="-0.4318" y2="0.4318" layer="51"/>
<rectangle x1="-0.1999" y1="-0.4001" x2="0.1999" y2="0.4001" layer="35"/>
</package>
<package name="R0805">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-0.41" y1="0.635" x2="0.41" y2="0.635" width="0.1524" layer="51"/>
<wire x1="-0.41" y1="-0.635" x2="0.41" y2="-0.635" width="0.1524" layer="51"/>
<wire x1="-1.973" y1="0.983" x2="1.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="0.983" x2="1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="-0.983" x2="-1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.973" y1="-0.983" x2="-1.973" y2="0.983" width="0.0508" layer="39"/>
<smd name="1" x="-0.85" y="0" dx="1.3" dy="1.5" layer="1"/>
<smd name="2" x="0.85" y="0" dx="1.3" dy="1.5" layer="1"/>
<text x="-0.762" y="1.016" size="1.27" layer="25">&gt;NAME</text>
<text x="-0.762" y="-2.286" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="0.4064" y1="-0.6985" x2="1.0564" y2="0.7015" layer="51"/>
<rectangle x1="-1.0668" y1="-0.6985" x2="-0.4168" y2="0.7015" layer="51"/>
<rectangle x1="-0.1999" y1="-0.5001" x2="0.1999" y2="0.5001" layer="35"/>
</package>
<package name="R0805W">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip, wave soldering</description>
<wire x1="-0.41" y1="0.635" x2="0.41" y2="0.635" width="0.1524" layer="51"/>
<wire x1="-0.41" y1="-0.635" x2="0.41" y2="-0.635" width="0.1524" layer="51"/>
<wire x1="-1.973" y1="0.983" x2="1.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="0.983" x2="1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="-0.983" x2="-1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.973" y1="-0.983" x2="-1.973" y2="0.983" width="0.0508" layer="39"/>
<smd name="1" x="-0.94" y="0" dx="1.5" dy="1" layer="1"/>
<smd name="2" x="0.94" y="0" dx="1.5" dy="1" layer="1"/>
<text x="-0.635" y="1.016" size="1.27" layer="25">&gt;NAME</text>
<text x="-0.635" y="-2.159" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="0.4064" y1="-0.6985" x2="1.0564" y2="0.7015" layer="51"/>
<rectangle x1="-1.0668" y1="-0.6985" x2="-0.4168" y2="0.7015" layer="51"/>
<rectangle x1="-0.1999" y1="-0.5001" x2="0.1999" y2="0.5001" layer="35"/>
</package>
<package name="R1005">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-0.245" y1="0.224" x2="0.245" y2="0.224" width="0.1524" layer="51"/>
<wire x1="0.245" y1="-0.224" x2="-0.245" y2="-0.224" width="0.1524" layer="51"/>
<wire x1="-1.473" y1="0.483" x2="1.473" y2="0.483" width="0.0508" layer="39"/>
<wire x1="1.473" y1="0.483" x2="1.473" y2="-0.483" width="0.0508" layer="39"/>
<wire x1="1.473" y1="-0.483" x2="-1.473" y2="-0.483" width="0.0508" layer="39"/>
<wire x1="-1.473" y1="-0.483" x2="-1.473" y2="0.483" width="0.0508" layer="39"/>
<smd name="1" x="-0.65" y="0" dx="0.7" dy="0.9" layer="1"/>
<smd name="2" x="0.65" y="0" dx="0.7" dy="0.9" layer="1"/>
<text x="-0.635" y="0.762" size="1.27" layer="25">&gt;NAME</text>
<text x="-0.635" y="-2.032" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-0.554" y1="-0.3048" x2="-0.254" y2="0.2951" layer="51"/>
<rectangle x1="0.2588" y1="-0.3048" x2="0.5588" y2="0.2951" layer="51"/>
<rectangle x1="-0.1999" y1="-0.3" x2="0.1999" y2="0.3" layer="35"/>
</package>
<package name="R1206">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="0.9525" y1="-0.8128" x2="-0.9652" y2="-0.8128" width="0.1524" layer="51"/>
<wire x1="0.9525" y1="0.8128" x2="-0.9652" y2="0.8128" width="0.1524" layer="51"/>
<wire x1="-2.473" y1="0.983" x2="2.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="0.983" x2="2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-0.983" x2="-2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-0.983" x2="-2.473" y2="0.983" width="0.0508" layer="39"/>
<smd name="2" x="1.422" y="0" dx="1.6" dy="1.803" layer="1"/>
<smd name="1" x="-1.422" y="0" dx="1.6" dy="1.803" layer="1"/>
<text x="-1.397" y="1.143" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.397" y="-2.413" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.6891" y1="-0.8763" x2="-0.9525" y2="0.8763" layer="51"/>
<rectangle x1="0.9525" y1="-0.8763" x2="1.6891" y2="0.8763" layer="51"/>
<rectangle x1="-0.3" y1="-0.7" x2="0.3" y2="0.7" layer="35"/>
</package>
<package name="R1206W">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip, wave soldering</description>
<wire x1="-0.913" y1="0.8" x2="0.888" y2="0.8" width="0.1524" layer="21"/>
<wire x1="-0.913" y1="-0.8" x2="0.888" y2="-0.8" width="0.1524" layer="21"/>
<wire x1="-2.473" y1="0.983" x2="2.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="0.983" x2="2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-0.983" x2="-2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-0.983" x2="-2.473" y2="0.983" width="0.0508" layer="39"/>
<smd name="1" x="-1.499" y="0" dx="1.8" dy="1.2" layer="1"/>
<smd name="2" x="1.499" y="0" dx="1.8" dy="1.2" layer="1"/>
<text x="-1.651" y="1.143" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.651" y="-2.413" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.651" y1="-0.8763" x2="-0.9009" y2="0.8738" layer="51"/>
<rectangle x1="0.889" y1="-0.8763" x2="1.6391" y2="0.8738" layer="51"/>
<rectangle x1="-0.3" y1="-0.7" x2="0.3" y2="0.7" layer="35"/>
</package>
<package name="R1210">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-0.913" y1="1.219" x2="0.939" y2="1.219" width="0.1524" layer="51"/>
<wire x1="-0.913" y1="-1.219" x2="0.939" y2="-1.219" width="0.1524" layer="51"/>
<wire x1="-2.473" y1="1.483" x2="2.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="1.483" x2="2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-1.483" x2="-2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-1.483" x2="-2.473" y2="1.483" width="0.0508" layer="39"/>
<smd name="1" x="-1.4" y="0" dx="1.6" dy="2.7" layer="1"/>
<smd name="2" x="1.4" y="0" dx="1.6" dy="2.7" layer="1"/>
<text x="-1.27" y="1.651" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.27" y="-2.921" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.651" y1="-1.3081" x2="-0.9009" y2="1.2918" layer="51"/>
<rectangle x1="0.9144" y1="-1.3081" x2="1.6645" y2="1.2918" layer="51"/>
<rectangle x1="-0.3" y1="-0.8999" x2="0.3" y2="0.8999" layer="35"/>
</package>
<package name="R1210W">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip, wave soldering</description>
<wire x1="-0.913" y1="1.219" x2="0.939" y2="1.219" width="0.1524" layer="21"/>
<wire x1="-0.913" y1="-1.219" x2="0.939" y2="-1.219" width="0.1524" layer="21"/>
<wire x1="-2.473" y1="1.483" x2="2.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="1.483" x2="2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-1.483" x2="-2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-1.483" x2="-2.473" y2="1.483" width="0.0508" layer="39"/>
<smd name="1" x="-1.499" y="0" dx="1.8" dy="1.8" layer="1"/>
<smd name="2" x="1.499" y="0" dx="1.8" dy="1.8" layer="1"/>
<text x="-1.651" y="1.524" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.651" y="-2.794" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.651" y1="-1.3081" x2="-0.9009" y2="1.2918" layer="51"/>
<rectangle x1="0.9144" y1="-1.3081" x2="1.6645" y2="1.2918" layer="51"/>
<rectangle x1="-0.3" y1="-0.8001" x2="0.3" y2="0.8001" layer="35"/>
</package>
<package name="R2010">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-1.662" y1="1.245" x2="1.662" y2="1.245" width="0.1524" layer="51"/>
<wire x1="-1.637" y1="-1.245" x2="1.687" y2="-1.245" width="0.1524" layer="51"/>
<wire x1="-3.473" y1="1.483" x2="3.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="3.473" y1="1.483" x2="3.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="3.473" y1="-1.483" x2="-3.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-3.473" y1="-1.483" x2="-3.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="-1.027" y1="1.245" x2="1.027" y2="1.245" width="0.1524" layer="21"/>
<wire x1="-1.002" y1="-1.245" x2="1.016" y2="-1.245" width="0.1524" layer="21"/>
<smd name="1" x="-2.2" y="0" dx="1.8" dy="2.7" layer="1"/>
<smd name="2" x="2.2" y="0" dx="1.8" dy="2.7" layer="1"/>
<text x="-2.159" y="1.651" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.159" y="-2.921" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-2.4892" y1="-1.3208" x2="-1.6393" y2="1.3292" layer="51"/>
<rectangle x1="1.651" y1="-1.3208" x2="2.5009" y2="1.3292" layer="51"/>
</package>
<package name="R2010W">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip, wave soldering</description>
<wire x1="-1.662" y1="1.245" x2="1.662" y2="1.245" width="0.1524" layer="21"/>
<wire x1="-1.637" y1="-1.245" x2="1.687" y2="-1.245" width="0.1524" layer="21"/>
<wire x1="-3.473" y1="1.483" x2="3.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="3.473" y1="1.483" x2="3.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="3.473" y1="-1.483" x2="-3.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-3.473" y1="-1.483" x2="-3.473" y2="1.483" width="0.0508" layer="39"/>
<smd name="1" x="-2.311" y="0" dx="2" dy="1.8" layer="1"/>
<smd name="2" x="2.311" y="0" dx="2" dy="1.8" layer="1"/>
<text x="-2.286" y="1.524" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.286" y="-2.921" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-2.4892" y1="-1.3208" x2="-1.6393" y2="1.3292" layer="51"/>
<rectangle x1="1.651" y1="-1.3208" x2="2.5009" y2="1.3292" layer="51"/>
</package>
<package name="R2012">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-0.41" y1="0.635" x2="0.41" y2="0.635" width="0.1524" layer="51"/>
<wire x1="-0.41" y1="-0.635" x2="0.41" y2="-0.635" width="0.1524" layer="51"/>
<wire x1="-1.973" y1="0.983" x2="1.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="0.983" x2="1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="-0.983" x2="-1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.973" y1="-0.983" x2="-1.973" y2="0.983" width="0.0508" layer="39"/>
<smd name="1" x="-0.85" y="0" dx="1.3" dy="1.5" layer="1"/>
<smd name="2" x="0.85" y="0" dx="1.3" dy="1.5" layer="1"/>
<text x="-0.762" y="1.016" size="1.27" layer="25">&gt;NAME</text>
<text x="-0.762" y="-2.286" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="0.4064" y1="-0.6985" x2="1.0564" y2="0.7015" layer="51"/>
<rectangle x1="-1.0668" y1="-0.6985" x2="-0.4168" y2="0.7015" layer="51"/>
<rectangle x1="-0.1001" y1="-0.5999" x2="0.1001" y2="0.5999" layer="35"/>
</package>
<package name="R2012W">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip, wave soldering</description>
<wire x1="-0.41" y1="0.635" x2="0.41" y2="0.635" width="0.1524" layer="51"/>
<wire x1="-0.41" y1="-0.635" x2="0.41" y2="-0.635" width="0.1524" layer="51"/>
<wire x1="-1.973" y1="0.983" x2="1.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="0.983" x2="1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="-0.983" x2="-1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.973" y1="-0.983" x2="-1.973" y2="0.983" width="0.0508" layer="39"/>
<smd name="1" x="-0.94" y="0" dx="1.5" dy="1" layer="1"/>
<smd name="2" x="0.94" y="0" dx="1.5" dy="1" layer="1"/>
<text x="-0.635" y="1.016" size="1.27" layer="25">&gt;NAME</text>
<text x="-0.635" y="-2.286" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="0.4064" y1="-0.6985" x2="1.0564" y2="0.7015" layer="51"/>
<rectangle x1="-1.0668" y1="-0.6985" x2="-0.4168" y2="0.7015" layer="51"/>
<rectangle x1="-0.1001" y1="-0.5999" x2="0.1001" y2="0.5999" layer="35"/>
</package>
<package name="R2512">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-2.362" y1="1.473" x2="2.387" y2="1.473" width="0.1524" layer="51"/>
<wire x1="-2.362" y1="-1.473" x2="2.387" y2="-1.473" width="0.1524" layer="51"/>
<wire x1="-3.973" y1="1.983" x2="3.973" y2="1.983" width="0.0508" layer="39"/>
<wire x1="3.973" y1="1.983" x2="3.973" y2="-1.983" width="0.0508" layer="39"/>
<wire x1="3.973" y1="-1.983" x2="-3.973" y2="-1.983" width="0.0508" layer="39"/>
<wire x1="-3.973" y1="-1.983" x2="-3.973" y2="1.983" width="0.0508" layer="39"/>
<wire x1="-1.473" y1="1.473" x2="1.498" y2="1.473" width="0.1524" layer="21"/>
<wire x1="-1.473" y1="-1.473" x2="1.498" y2="-1.473" width="0.1524" layer="21"/>
<smd name="1" x="-2.8" y="0" dx="1.8" dy="3.2" layer="1"/>
<smd name="2" x="2.8" y="0" dx="1.8" dy="3.2" layer="1"/>
<text x="-2.667" y="1.905" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.667" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-3.2004" y1="-1.5494" x2="-2.3505" y2="1.5507" layer="51"/>
<rectangle x1="2.3622" y1="-1.5494" x2="3.2121" y2="1.5507" layer="51"/>
<rectangle x1="-0.5001" y1="-1" x2="0.5001" y2="1" layer="35"/>
</package>
<package name="R2512W">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip, wave soldering</description>
<wire x1="-2.362" y1="1.473" x2="2.387" y2="1.473" width="0.1524" layer="21"/>
<wire x1="-2.362" y1="-1.473" x2="2.387" y2="-1.473" width="0.1524" layer="21"/>
<wire x1="-3.973" y1="1.983" x2="3.973" y2="1.983" width="0.0508" layer="39"/>
<wire x1="3.973" y1="1.983" x2="3.973" y2="-1.983" width="0.0508" layer="39"/>
<wire x1="3.973" y1="-1.983" x2="-3.973" y2="-1.983" width="0.0508" layer="39"/>
<wire x1="-3.973" y1="-1.983" x2="-3.973" y2="1.983" width="0.0508" layer="39"/>
<smd name="1" x="-2.896" y="0" dx="2" dy="2.1" layer="1"/>
<smd name="2" x="2.896" y="0" dx="2" dy="2.1" layer="1"/>
<text x="-2.794" y="1.778" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.794" y="-3.048" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-3.2004" y1="-1.5494" x2="-2.3505" y2="1.5507" layer="51"/>
<rectangle x1="2.3622" y1="-1.5494" x2="3.2121" y2="1.5507" layer="51"/>
<rectangle x1="-0.5001" y1="-1" x2="0.5001" y2="1" layer="35"/>
</package>
<package name="R3216">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-0.913" y1="0.8" x2="0.888" y2="0.8" width="0.1524" layer="51"/>
<wire x1="-0.913" y1="-0.8" x2="0.888" y2="-0.8" width="0.1524" layer="51"/>
<wire x1="-2.473" y1="0.983" x2="2.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="0.983" x2="2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-0.983" x2="-2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-0.983" x2="-2.473" y2="0.983" width="0.0508" layer="39"/>
<smd name="1" x="-1.4" y="0" dx="1.6" dy="1.8" layer="1"/>
<smd name="2" x="1.4" y="0" dx="1.6" dy="1.8" layer="1"/>
<text x="-1.397" y="1.143" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.397" y="-2.413" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.651" y1="-0.8763" x2="-0.9009" y2="0.8738" layer="51"/>
<rectangle x1="0.889" y1="-0.8763" x2="1.6391" y2="0.8738" layer="51"/>
<rectangle x1="-0.3" y1="-0.7" x2="0.3" y2="0.7" layer="35"/>
</package>
<package name="R3216W">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip, wave soldering</description>
<wire x1="-0.913" y1="0.8" x2="0.888" y2="0.8" width="0.1524" layer="21"/>
<wire x1="-0.913" y1="-0.8" x2="0.888" y2="-0.8" width="0.1524" layer="21"/>
<wire x1="-2.473" y1="0.983" x2="2.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="0.983" x2="2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-0.983" x2="-2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-0.983" x2="-2.473" y2="0.983" width="0.0508" layer="39"/>
<smd name="1" x="-1.499" y="0" dx="1.8" dy="1.2" layer="1"/>
<smd name="2" x="1.499" y="0" dx="1.8" dy="1.2" layer="1"/>
<text x="-1.524" y="1.143" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.524" y="-2.413" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.651" y1="-0.8763" x2="-0.9009" y2="0.8738" layer="51"/>
<rectangle x1="0.889" y1="-0.8763" x2="1.6391" y2="0.8738" layer="51"/>
<rectangle x1="-0.3" y1="-0.7" x2="0.3" y2="0.7" layer="35"/>
</package>
<package name="R3225">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-0.913" y1="1.219" x2="0.939" y2="1.219" width="0.1524" layer="51"/>
<wire x1="-0.913" y1="-1.219" x2="0.939" y2="-1.219" width="0.1524" layer="51"/>
<wire x1="-2.473" y1="1.483" x2="2.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="1.483" x2="2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-1.483" x2="-2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-1.483" x2="-2.473" y2="1.483" width="0.0508" layer="39"/>
<smd name="1" x="-1.4" y="0" dx="1.6" dy="2.7" layer="1"/>
<smd name="2" x="1.4" y="0" dx="1.6" dy="2.7" layer="1"/>
<text x="-1.397" y="1.651" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.397" y="-2.921" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.651" y1="-1.3081" x2="-0.9009" y2="1.2918" layer="51"/>
<rectangle x1="0.9144" y1="-1.3081" x2="1.6645" y2="1.2918" layer="51"/>
<rectangle x1="-0.3" y1="-1" x2="0.3" y2="1" layer="35"/>
</package>
<package name="R3225W">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip, wave soldering</description>
<wire x1="-0.913" y1="1.219" x2="0.939" y2="1.219" width="0.1524" layer="21"/>
<wire x1="-0.913" y1="-1.219" x2="0.939" y2="-1.219" width="0.1524" layer="21"/>
<wire x1="-2.473" y1="1.483" x2="2.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="1.483" x2="2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-1.483" x2="-2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-1.483" x2="-2.473" y2="1.483" width="0.0508" layer="39"/>
<smd name="1" x="-1.499" y="0" dx="1.8" dy="1.8" layer="1"/>
<smd name="2" x="1.499" y="0" dx="1.8" dy="1.8" layer="1"/>
<text x="-1.397" y="1.524" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.397" y="-2.921" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.651" y1="-1.3081" x2="-0.9009" y2="1.2918" layer="51"/>
<rectangle x1="0.9144" y1="-1.3081" x2="1.6645" y2="1.2918" layer="51"/>
<rectangle x1="-0.3" y1="-1" x2="0.3" y2="1" layer="35"/>
</package>
<package name="R5025">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-1.662" y1="1.245" x2="1.662" y2="1.245" width="0.1524" layer="51"/>
<wire x1="-1.637" y1="-1.245" x2="1.687" y2="-1.245" width="0.1524" layer="51"/>
<wire x1="-3.473" y1="1.483" x2="3.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="3.473" y1="1.483" x2="3.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="3.473" y1="-1.483" x2="-3.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-3.473" y1="-1.483" x2="-3.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="-0.9" y1="1.245" x2="0.9" y2="1.245" width="0.1524" layer="21"/>
<wire x1="-0.875" y1="-1.245" x2="0.925" y2="-1.245" width="0.1524" layer="21"/>
<smd name="1" x="-2.2" y="0" dx="1.8" dy="2.7" layer="1"/>
<smd name="2" x="2.2" y="0" dx="1.8" dy="2.7" layer="1"/>
<text x="-2.159" y="1.651" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.159" y="-2.921" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-2.4892" y1="-1.3208" x2="-1.6393" y2="1.3292" layer="51"/>
<rectangle x1="1.651" y1="-1.3208" x2="2.5009" y2="1.3292" layer="51"/>
<rectangle x1="-0.5001" y1="-1" x2="0.5001" y2="1" layer="35"/>
</package>
<package name="R5025W">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip, wave soldering</description>
<wire x1="-1.662" y1="1.245" x2="1.662" y2="1.245" width="0.1524" layer="21"/>
<wire x1="-1.637" y1="-1.245" x2="1.687" y2="-1.245" width="0.1524" layer="21"/>
<wire x1="-3.473" y1="1.483" x2="3.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="3.473" y1="1.483" x2="3.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="3.473" y1="-1.483" x2="-3.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-3.473" y1="-1.483" x2="-3.473" y2="1.483" width="0.0508" layer="39"/>
<smd name="1" x="-2.311" y="0" dx="2" dy="1.8" layer="1"/>
<smd name="2" x="2.311" y="0" dx="2" dy="1.8" layer="1"/>
<text x="-2.286" y="1.651" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.286" y="-2.921" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-2.4892" y1="-1.3208" x2="-1.6393" y2="1.3292" layer="51"/>
<rectangle x1="1.651" y1="-1.3208" x2="2.5009" y2="1.3292" layer="51"/>
<rectangle x1="-0.5001" y1="-1" x2="0.5001" y2="1" layer="35"/>
</package>
<package name="R6332">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip</description>
<wire x1="-2.362" y1="1.473" x2="2.387" y2="1.473" width="0.1524" layer="51"/>
<wire x1="-2.362" y1="-1.473" x2="2.387" y2="-1.473" width="0.1524" layer="51"/>
<wire x1="-3.973" y1="1.983" x2="3.973" y2="1.983" width="0.0508" layer="39"/>
<wire x1="3.973" y1="1.983" x2="3.973" y2="-1.983" width="0.0508" layer="39"/>
<wire x1="3.973" y1="-1.983" x2="-3.973" y2="-1.983" width="0.0508" layer="39"/>
<wire x1="-3.973" y1="-1.983" x2="-3.973" y2="1.983" width="0.0508" layer="39"/>
<wire x1="-1.473" y1="1.473" x2="1.498" y2="1.473" width="0.1524" layer="21"/>
<wire x1="-1.473" y1="-1.473" x2="1.498" y2="-1.473" width="0.1524" layer="21"/>
<smd name="1" x="-2.8" y="0" dx="1.8" dy="3.2" layer="1"/>
<smd name="2" x="2.8" y="0" dx="1.8" dy="3.2" layer="1"/>
<text x="-2.794" y="1.905" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.794" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-3.2004" y1="-1.5494" x2="-2.3505" y2="1.5507" layer="51"/>
<rectangle x1="2.3622" y1="-1.5494" x2="3.2121" y2="1.5507" layer="51"/>
<rectangle x1="-0.5001" y1="-1" x2="0.5001" y2="1" layer="35"/>
</package>
<package name="R6332W">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
chip, wave soldering</description>
<wire x1="-2.362" y1="1.473" x2="2.387" y2="1.473" width="0.1524" layer="21"/>
<wire x1="-2.362" y1="-1.473" x2="2.387" y2="-1.473" width="0.1524" layer="21"/>
<wire x1="-3.973" y1="1.983" x2="3.973" y2="1.983" width="0.0508" layer="39"/>
<wire x1="3.973" y1="1.983" x2="3.973" y2="-1.983" width="0.0508" layer="39"/>
<wire x1="3.973" y1="-1.983" x2="-3.973" y2="-1.983" width="0.0508" layer="39"/>
<wire x1="-3.973" y1="-1.983" x2="-3.973" y2="1.983" width="0.0508" layer="39"/>
<smd name="1" x="-2.896" y="0" dx="2" dy="2.1" layer="1"/>
<smd name="2" x="2.896" y="0" dx="2" dy="2.1" layer="1"/>
<text x="-2.921" y="1.778" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.921" y="-3.048" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-3.2004" y1="-1.5494" x2="-2.3505" y2="1.5507" layer="51"/>
<rectangle x1="2.3622" y1="-1.5494" x2="3.2121" y2="1.5507" layer="51"/>
<rectangle x1="-0.5001" y1="-1" x2="0.5001" y2="1" layer="35"/>
</package>
<package name="M0805">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
MELF 0.10 W</description>
<wire x1="-1.973" y1="0.983" x2="1.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="-0.983" x2="-1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.973" y1="-0.983" x2="-1.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="0.983" x2="1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="0.7112" y1="0.635" x2="-0.7112" y2="0.635" width="0.1524" layer="51"/>
<wire x1="0.7112" y1="-0.635" x2="-0.7112" y2="-0.635" width="0.1524" layer="51"/>
<smd name="1" x="-0.95" y="0" dx="1.3" dy="1.6" layer="1"/>
<smd name="2" x="0.95" y="0" dx="1.3" dy="1.6" layer="1"/>
<text x="-1.016" y="1.016" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.016" y="-2.286" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.0414" y1="-0.7112" x2="-0.6858" y2="0.7112" layer="51"/>
<rectangle x1="0.6858" y1="-0.7112" x2="1.0414" y2="0.7112" layer="51"/>
<rectangle x1="-0.1999" y1="-0.5999" x2="0.1999" y2="0.5999" layer="35"/>
</package>
<package name="M1206">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
MELF 0.25 W</description>
<wire x1="-2.473" y1="1.483" x2="2.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-1.483" x2="-2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-1.483" x2="-2.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="1.483" x2="2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="1.143" y1="0.8382" x2="-1.143" y2="0.8382" width="0.1524" layer="51"/>
<wire x1="1.143" y1="-0.8382" x2="-1.143" y2="-0.8382" width="0.1524" layer="51"/>
<smd name="1" x="-1.4" y="0" dx="1.6" dy="2" layer="1"/>
<smd name="2" x="1.4" y="0" dx="1.6" dy="2" layer="1"/>
<text x="-1.27" y="1.27" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.27" y="-2.54" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.7018" y1="-0.9144" x2="-1.1176" y2="0.9144" layer="51"/>
<rectangle x1="1.1176" y1="-0.9144" x2="1.7018" y2="0.9144" layer="51"/>
<rectangle x1="-0.3" y1="-0.8001" x2="0.3" y2="0.8001" layer="35"/>
</package>
<package name="M1406">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
MELF 0.12 W</description>
<wire x1="-2.973" y1="0.983" x2="2.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="2.973" y1="-0.983" x2="-2.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-2.973" y1="-0.983" x2="-2.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="2.973" y1="0.983" x2="2.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="1.3208" y1="0.762" x2="-1.3208" y2="0.762" width="0.1524" layer="51"/>
<wire x1="1.3208" y1="-0.762" x2="-1.3208" y2="-0.762" width="0.1524" layer="51"/>
<wire x1="0.6858" y1="0.762" x2="-0.6858" y2="0.762" width="0.1524" layer="21"/>
<wire x1="0.6858" y1="-0.762" x2="-0.6858" y2="-0.762" width="0.1524" layer="21"/>
<smd name="1" x="-1.7" y="0" dx="1.4" dy="1.8" layer="1"/>
<smd name="2" x="1.7" y="0" dx="1.4" dy="1.8" layer="1"/>
<text x="-1.651" y="1.143" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.651" y="-2.413" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.8542" y1="-0.8382" x2="-1.2954" y2="0.8382" layer="51"/>
<rectangle x1="1.2954" y1="-0.8382" x2="1.8542" y2="0.8382" layer="51"/>
<rectangle x1="-0.3" y1="-0.7" x2="0.3" y2="0.7" layer="35"/>
</package>
<package name="M2012">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
MELF 0.10 W</description>
<wire x1="-1.973" y1="0.983" x2="1.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="-0.983" x2="-1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-1.973" y1="-0.983" x2="-1.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="1.973" y1="0.983" x2="1.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="0.7112" y1="0.635" x2="-0.7112" y2="0.635" width="0.1524" layer="51"/>
<wire x1="0.7112" y1="-0.635" x2="-0.7112" y2="-0.635" width="0.1524" layer="51"/>
<smd name="1" x="-0.95" y="0" dx="1.3" dy="1.6" layer="1"/>
<smd name="2" x="0.95" y="0" dx="1.3" dy="1.6" layer="1"/>
<text x="-1.016" y="1.016" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.016" y="-2.286" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.0414" y1="-0.7112" x2="-0.6858" y2="0.7112" layer="51"/>
<rectangle x1="0.6858" y1="-0.7112" x2="1.0414" y2="0.7112" layer="51"/>
<rectangle x1="-0.1999" y1="-0.5999" x2="0.1999" y2="0.5999" layer="35"/>
</package>
<package name="M2309">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
MELF 0.25 W</description>
<wire x1="-4.473" y1="1.483" x2="4.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="4.473" y1="-1.483" x2="-4.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-4.473" y1="-1.483" x2="-4.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="4.473" y1="1.483" x2="4.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="2.413" y1="1.1684" x2="-2.4384" y2="1.1684" width="0.1524" layer="51"/>
<wire x1="2.413" y1="-1.1684" x2="-2.413" y2="-1.1684" width="0.1524" layer="51"/>
<wire x1="1.651" y1="1.1684" x2="-1.6764" y2="1.1684" width="0.1524" layer="21"/>
<wire x1="1.651" y1="-1.1684" x2="-1.651" y2="-1.1684" width="0.1524" layer="21"/>
<smd name="1" x="-2.85" y="0" dx="1.5" dy="2.6" layer="1"/>
<smd name="2" x="2.85" y="0" dx="1.5" dy="2.6" layer="1"/>
<text x="-2.794" y="1.651" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.794" y="-2.794" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-3.048" y1="-1.2446" x2="-2.3876" y2="1.2446" layer="51"/>
<rectangle x1="2.3876" y1="-1.2446" x2="3.048" y2="1.2446" layer="51"/>
<rectangle x1="-0.5001" y1="-1" x2="0.5001" y2="1" layer="35"/>
</package>
<package name="M3216">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
MELF 0.25 W</description>
<wire x1="-2.473" y1="1.483" x2="2.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-1.483" x2="-2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-1.483" x2="-2.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="2.473" y1="1.483" x2="2.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="1.143" y1="0.8382" x2="-1.143" y2="0.8382" width="0.1524" layer="51"/>
<wire x1="1.143" y1="-0.8382" x2="-1.143" y2="-0.8382" width="0.1524" layer="51"/>
<smd name="1" x="-1.4" y="0" dx="1.6" dy="2" layer="1"/>
<smd name="2" x="1.4" y="0" dx="1.6" dy="2" layer="1"/>
<text x="-1.27" y="1.27" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.27" y="-2.54" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.7018" y1="-0.9144" x2="-1.1176" y2="0.9144" layer="51"/>
<rectangle x1="1.1176" y1="-0.9144" x2="1.7018" y2="0.9144" layer="51"/>
<rectangle x1="-0.3" y1="-0.8001" x2="0.3" y2="0.8001" layer="35"/>
</package>
<package name="M3516">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
MELF 0.12 W</description>
<wire x1="-2.973" y1="0.983" x2="2.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="2.973" y1="-0.983" x2="-2.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-2.973" y1="-0.983" x2="-2.973" y2="0.983" width="0.0508" layer="39"/>
<wire x1="2.973" y1="0.983" x2="2.973" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="1.3208" y1="0.762" x2="-1.3208" y2="0.762" width="0.1524" layer="51"/>
<wire x1="1.3208" y1="-0.762" x2="-1.3208" y2="-0.762" width="0.1524" layer="51"/>
<wire x1="0.6858" y1="0.762" x2="-0.6858" y2="0.762" width="0.1524" layer="21"/>
<wire x1="0.6858" y1="-0.762" x2="-0.6858" y2="-0.762" width="0.1524" layer="21"/>
<smd name="1" x="-1.7" y="0" dx="1.4" dy="1.8" layer="1"/>
<smd name="2" x="1.7" y="0" dx="1.4" dy="1.8" layer="1"/>
<text x="-1.651" y="1.143" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.651" y="-2.413" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.8542" y1="-0.8382" x2="-1.2954" y2="0.8382" layer="51"/>
<rectangle x1="1.2954" y1="-0.8382" x2="1.8542" y2="0.8382" layer="51"/>
<rectangle x1="-0.4001" y1="-0.7" x2="0.4001" y2="0.7" layer="35"/>
</package>
<package name="M5923">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
MELF 0.25 W</description>
<wire x1="-4.473" y1="1.483" x2="4.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="4.473" y1="-1.483" x2="-4.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="-4.473" y1="-1.483" x2="-4.473" y2="1.483" width="0.0508" layer="39"/>
<wire x1="4.473" y1="1.483" x2="4.473" y2="-1.483" width="0.0508" layer="39"/>
<wire x1="2.413" y1="1.1684" x2="-2.4384" y2="1.1684" width="0.1524" layer="51"/>
<wire x1="2.413" y1="-1.1684" x2="-2.413" y2="-1.1684" width="0.1524" layer="51"/>
<wire x1="1.651" y1="1.1684" x2="-1.6764" y2="1.1684" width="0.1524" layer="21"/>
<wire x1="1.651" y1="-1.1684" x2="-1.651" y2="-1.1684" width="0.1524" layer="21"/>
<smd name="1" x="-2.85" y="0" dx="1.5" dy="2.6" layer="1"/>
<smd name="2" x="2.85" y="0" dx="1.5" dy="2.6" layer="1"/>
<text x="-2.794" y="1.651" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.794" y="-2.794" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-3.048" y1="-1.2446" x2="-2.3876" y2="1.2446" layer="51"/>
<rectangle x1="2.3876" y1="-1.2446" x2="3.048" y2="1.2446" layer="51"/>
<rectangle x1="-0.5001" y1="-1" x2="0.5001" y2="1" layer="35"/>
</package>
<package name="0204/5">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0204, grid 5 mm</description>
<wire x1="2.54" y1="0" x2="2.032" y2="0" width="0.508" layer="51"/>
<wire x1="-2.54" y1="0" x2="-2.032" y2="0" width="0.508" layer="51"/>
<wire x1="-1.778" y1="0.635" x2="-1.524" y2="0.889" width="0.1524" layer="21" curve="-90"/>
<wire x1="-1.778" y1="-0.635" x2="-1.524" y2="-0.889" width="0.1524" layer="21" curve="90"/>
<wire x1="1.524" y1="-0.889" x2="1.778" y2="-0.635" width="0.1524" layer="21" curve="90"/>
<wire x1="1.524" y1="0.889" x2="1.778" y2="0.635" width="0.1524" layer="21" curve="-90"/>
<wire x1="-1.778" y1="-0.635" x2="-1.778" y2="0.635" width="0.1524" layer="51"/>
<wire x1="-1.524" y1="0.889" x2="-1.27" y2="0.889" width="0.1524" layer="21"/>
<wire x1="-1.143" y1="0.762" x2="-1.27" y2="0.889" width="0.1524" layer="21"/>
<wire x1="-1.524" y1="-0.889" x2="-1.27" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="-1.143" y1="-0.762" x2="-1.27" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="1.143" y1="0.762" x2="1.27" y2="0.889" width="0.1524" layer="21"/>
<wire x1="1.143" y1="0.762" x2="-1.143" y2="0.762" width="0.1524" layer="21"/>
<wire x1="1.143" y1="-0.762" x2="1.27" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="1.143" y1="-0.762" x2="-1.143" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="1.524" y1="0.889" x2="1.27" y2="0.889" width="0.1524" layer="21"/>
<wire x1="1.524" y1="-0.889" x2="1.27" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="1.778" y1="-0.635" x2="1.778" y2="0.635" width="0.1524" layer="51"/>
<pad name="1" x="-2.54" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="2.54" y="0" drill="0.8128" shape="octagon"/>
<text x="-2.0066" y="1.1684" size="0.9906" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.1336" y="-2.3114" size="0.9906" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-2.032" y1="-0.254" x2="-1.778" y2="0.254" layer="51"/>
<rectangle x1="1.778" y1="-0.254" x2="2.032" y2="0.254" layer="51"/>
</package>
<package name="0204/7">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0204, grid 7.5 mm</description>
<wire x1="3.81" y1="0" x2="2.921" y2="0" width="0.508" layer="51"/>
<wire x1="-3.81" y1="0" x2="-2.921" y2="0" width="0.508" layer="51"/>
<wire x1="-2.54" y1="0.762" x2="-2.286" y2="1.016" width="0.1524" layer="21" curve="-90"/>
<wire x1="-2.54" y1="-0.762" x2="-2.286" y2="-1.016" width="0.1524" layer="21" curve="90"/>
<wire x1="2.286" y1="-1.016" x2="2.54" y2="-0.762" width="0.1524" layer="21" curve="90"/>
<wire x1="2.286" y1="1.016" x2="2.54" y2="0.762" width="0.1524" layer="21" curve="-90"/>
<wire x1="-2.54" y1="-0.762" x2="-2.54" y2="0.762" width="0.1524" layer="21"/>
<wire x1="-2.286" y1="1.016" x2="-1.905" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-1.778" y1="0.889" x2="-1.905" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-2.286" y1="-1.016" x2="-1.905" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="-1.778" y1="-0.889" x2="-1.905" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="1.778" y1="0.889" x2="1.905" y2="1.016" width="0.1524" layer="21"/>
<wire x1="1.778" y1="0.889" x2="-1.778" y2="0.889" width="0.1524" layer="21"/>
<wire x1="1.778" y1="-0.889" x2="1.905" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="1.778" y1="-0.889" x2="-1.778" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="2.286" y1="1.016" x2="1.905" y2="1.016" width="0.1524" layer="21"/>
<wire x1="2.286" y1="-1.016" x2="1.905" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-0.762" x2="2.54" y2="0.762" width="0.1524" layer="21"/>
<pad name="1" x="-3.81" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="3.81" y="0" drill="0.8128" shape="octagon"/>
<text x="-2.54" y="1.2954" size="0.9906" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.6256" y="-0.4826" size="0.9906" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="2.54" y1="-0.254" x2="2.921" y2="0.254" layer="21"/>
<rectangle x1="-2.921" y1="-0.254" x2="-2.54" y2="0.254" layer="21"/>
</package>
<package name="0207/10">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0207, grid 10 mm</description>
<wire x1="5.08" y1="0" x2="4.064" y2="0" width="0.6096" layer="51"/>
<wire x1="-5.08" y1="0" x2="-4.064" y2="0" width="0.6096" layer="51"/>
<wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
<wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
<wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
<wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
<wire x1="-3.175" y1="-0.889" x2="-3.175" y2="0.889" width="0.1524" layer="21"/>
<wire x1="-2.921" y1="1.143" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="-2.413" y1="1.016" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="-2.921" y1="-1.143" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="-2.413" y1="-1.016" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="1.016" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="1.016" x2="-2.413" y2="1.016" width="0.1524" layer="21"/>
<wire x1="2.413" y1="-1.016" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="-1.016" x2="-2.413" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="2.921" y1="1.143" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="2.921" y1="-1.143" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-0.889" x2="3.175" y2="0.889" width="0.1524" layer="21"/>
<pad name="1" x="-5.08" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="5.08" y="0" drill="0.8128" shape="octagon"/>
<text x="-3.048" y="1.524" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.2606" y="-0.635" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="3.175" y1="-0.3048" x2="4.0386" y2="0.3048" layer="21"/>
<rectangle x1="-4.0386" y1="-0.3048" x2="-3.175" y2="0.3048" layer="21"/>
</package>
<package name="0207/12">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0207, grid 12 mm</description>
<wire x1="6.35" y1="0" x2="5.334" y2="0" width="0.6096" layer="51"/>
<wire x1="-6.35" y1="0" x2="-5.334" y2="0" width="0.6096" layer="51"/>
<wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
<wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
<wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
<wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
<wire x1="-3.175" y1="-0.889" x2="-3.175" y2="0.889" width="0.1524" layer="21"/>
<wire x1="-2.921" y1="1.143" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="-2.413" y1="1.016" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="-2.921" y1="-1.143" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="-2.413" y1="-1.016" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="1.016" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="1.016" x2="-2.413" y2="1.016" width="0.1524" layer="21"/>
<wire x1="2.413" y1="-1.016" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="-1.016" x2="-2.413" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="2.921" y1="1.143" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="2.921" y1="-1.143" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-0.889" x2="3.175" y2="0.889" width="0.1524" layer="21"/>
<wire x1="4.445" y1="0" x2="4.064" y2="0" width="0.6096" layer="21"/>
<wire x1="-4.445" y1="0" x2="-4.064" y2="0" width="0.6096" layer="21"/>
<pad name="1" x="-6.35" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="6.35" y="0" drill="0.8128" shape="octagon"/>
<text x="-3.175" y="1.397" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.286" y="-0.6858" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="3.175" y1="-0.3048" x2="4.0386" y2="0.3048" layer="21"/>
<rectangle x1="-4.0386" y1="-0.3048" x2="-3.175" y2="0.3048" layer="21"/>
<rectangle x1="4.445" y1="-0.3048" x2="5.3086" y2="0.3048" layer="21"/>
<rectangle x1="-5.3086" y1="-0.3048" x2="-4.445" y2="0.3048" layer="21"/>
</package>
<package name="0207/15">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0207, grid 15mm</description>
<wire x1="7.62" y1="0" x2="6.604" y2="0" width="0.6096" layer="51"/>
<wire x1="-7.62" y1="0" x2="-6.604" y2="0" width="0.6096" layer="51"/>
<wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
<wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
<wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
<wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
<wire x1="-3.175" y1="-0.889" x2="-3.175" y2="0.889" width="0.1524" layer="21"/>
<wire x1="-2.921" y1="1.143" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="-2.413" y1="1.016" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="-2.921" y1="-1.143" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="-2.413" y1="-1.016" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="1.016" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="1.016" x2="-2.413" y2="1.016" width="0.1524" layer="21"/>
<wire x1="2.413" y1="-1.016" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="-1.016" x2="-2.413" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="2.921" y1="1.143" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="2.921" y1="-1.143" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-0.889" x2="3.175" y2="0.889" width="0.1524" layer="21"/>
<wire x1="5.715" y1="0" x2="4.064" y2="0" width="0.6096" layer="21"/>
<wire x1="-5.715" y1="0" x2="-4.064" y2="0" width="0.6096" layer="21"/>
<pad name="1" x="-7.62" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="7.62" y="0" drill="0.8128" shape="octagon"/>
<text x="-3.175" y="1.397" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.286" y="-0.6858" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="3.175" y1="-0.3048" x2="4.0386" y2="0.3048" layer="21"/>
<rectangle x1="-4.0386" y1="-0.3048" x2="-3.175" y2="0.3048" layer="21"/>
<rectangle x1="5.715" y1="-0.3048" x2="6.5786" y2="0.3048" layer="21"/>
<rectangle x1="-6.5786" y1="-0.3048" x2="-5.715" y2="0.3048" layer="21"/>
</package>
<package name="0207/2V">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0207, grid 2.5 mm</description>
<wire x1="-1.27" y1="0" x2="-0.381" y2="0" width="0.6096" layer="51"/>
<wire x1="-0.254" y1="0" x2="0.254" y2="0" width="0.6096" layer="21"/>
<wire x1="0.381" y1="0" x2="1.27" y2="0" width="0.6096" layer="51"/>
<circle x="-1.27" y="0" radius="1.27" width="0.1524" layer="21"/>
<circle x="-1.27" y="0" radius="1.016" width="0.1524" layer="51"/>
<pad name="1" x="-1.27" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="1.27" y="0" drill="0.8128" shape="octagon"/>
<text x="-0.0508" y="1.016" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-0.0508" y="-2.2352" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="0207/5V">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0207, grid 5 mm</description>
<wire x1="-2.54" y1="0" x2="-0.889" y2="0" width="0.6096" layer="51"/>
<wire x1="-0.762" y1="0" x2="0.762" y2="0" width="0.6096" layer="21"/>
<wire x1="0.889" y1="0" x2="2.54" y2="0" width="0.6096" layer="51"/>
<circle x="-2.54" y="0" radius="1.27" width="0.1016" layer="21"/>
<circle x="-2.54" y="0" radius="1.016" width="0.1524" layer="51"/>
<pad name="1" x="-2.54" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="2.54" y="0" drill="0.8128" shape="octagon"/>
<text x="-1.143" y="0.889" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.143" y="-2.159" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="0207/7">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0207, grid 7.5 mm</description>
<wire x1="-3.81" y1="0" x2="-3.429" y2="0" width="0.6096" layer="51"/>
<wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
<wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
<wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
<wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
<wire x1="-3.175" y1="-0.889" x2="-3.175" y2="0.889" width="0.1524" layer="51"/>
<wire x1="-2.921" y1="1.143" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="-2.413" y1="1.016" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="-2.921" y1="-1.143" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="-2.413" y1="-1.016" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="1.016" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="1.016" x2="-2.413" y2="1.016" width="0.1524" layer="21"/>
<wire x1="2.413" y1="-1.016" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="-1.016" x2="-2.413" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="2.921" y1="1.143" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="2.921" y1="-1.143" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-0.889" x2="3.175" y2="0.889" width="0.1524" layer="51"/>
<wire x1="3.429" y1="0" x2="3.81" y2="0" width="0.6096" layer="51"/>
<pad name="1" x="-3.81" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="3.81" y="0" drill="0.8128" shape="octagon"/>
<text x="-2.54" y="1.397" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.286" y="-0.5588" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-3.429" y1="-0.3048" x2="-3.175" y2="0.3048" layer="51"/>
<rectangle x1="3.175" y1="-0.3048" x2="3.429" y2="0.3048" layer="51"/>
</package>
<package name="0309/10">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0309, grid 10mm</description>
<wire x1="-4.699" y1="0" x2="-5.08" y2="0" width="0.6096" layer="51"/>
<wire x1="-4.318" y1="1.27" x2="-4.064" y2="1.524" width="0.1524" layer="21" curve="-90"/>
<wire x1="-4.318" y1="-1.27" x2="-4.064" y2="-1.524" width="0.1524" layer="21" curve="90"/>
<wire x1="4.064" y1="-1.524" x2="4.318" y2="-1.27" width="0.1524" layer="21" curve="90"/>
<wire x1="4.064" y1="1.524" x2="4.318" y2="1.27" width="0.1524" layer="21" curve="-90"/>
<wire x1="-4.318" y1="-1.27" x2="-4.318" y2="1.27" width="0.1524" layer="51"/>
<wire x1="-4.064" y1="1.524" x2="-3.429" y2="1.524" width="0.1524" layer="21"/>
<wire x1="-3.302" y1="1.397" x2="-3.429" y2="1.524" width="0.1524" layer="21"/>
<wire x1="-4.064" y1="-1.524" x2="-3.429" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="-3.302" y1="-1.397" x2="-3.429" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="3.302" y1="1.397" x2="3.429" y2="1.524" width="0.1524" layer="21"/>
<wire x1="3.302" y1="1.397" x2="-3.302" y2="1.397" width="0.1524" layer="21"/>
<wire x1="3.302" y1="-1.397" x2="3.429" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="3.302" y1="-1.397" x2="-3.302" y2="-1.397" width="0.1524" layer="21"/>
<wire x1="4.064" y1="1.524" x2="3.429" y2="1.524" width="0.1524" layer="21"/>
<wire x1="4.064" y1="-1.524" x2="3.429" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="4.318" y1="-1.27" x2="4.318" y2="1.27" width="0.1524" layer="51"/>
<wire x1="5.08" y1="0" x2="4.699" y2="0" width="0.6096" layer="51"/>
<pad name="1" x="-5.08" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="5.08" y="0" drill="0.8128" shape="octagon"/>
<text x="-4.191" y="1.905" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.175" y="-0.6858" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-4.6228" y1="-0.3048" x2="-4.318" y2="0.3048" layer="51"/>
<rectangle x1="4.318" y1="-0.3048" x2="4.6228" y2="0.3048" layer="51"/>
</package>
<package name="0309/12">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0309, grid 12.5 mm</description>
<wire x1="6.35" y1="0" x2="5.08" y2="0" width="0.6096" layer="51"/>
<wire x1="-6.35" y1="0" x2="-5.08" y2="0" width="0.6096" layer="51"/>
<wire x1="-4.318" y1="1.27" x2="-4.064" y2="1.524" width="0.1524" layer="21" curve="-90"/>
<wire x1="-4.318" y1="-1.27" x2="-4.064" y2="-1.524" width="0.1524" layer="21" curve="90"/>
<wire x1="4.064" y1="-1.524" x2="4.318" y2="-1.27" width="0.1524" layer="21" curve="90"/>
<wire x1="4.064" y1="1.524" x2="4.318" y2="1.27" width="0.1524" layer="21" curve="-90"/>
<wire x1="-4.318" y1="-1.27" x2="-4.318" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-4.064" y1="1.524" x2="-3.429" y2="1.524" width="0.1524" layer="21"/>
<wire x1="-3.302" y1="1.397" x2="-3.429" y2="1.524" width="0.1524" layer="21"/>
<wire x1="-4.064" y1="-1.524" x2="-3.429" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="-3.302" y1="-1.397" x2="-3.429" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="3.302" y1="1.397" x2="3.429" y2="1.524" width="0.1524" layer="21"/>
<wire x1="3.302" y1="1.397" x2="-3.302" y2="1.397" width="0.1524" layer="21"/>
<wire x1="3.302" y1="-1.397" x2="3.429" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="3.302" y1="-1.397" x2="-3.302" y2="-1.397" width="0.1524" layer="21"/>
<wire x1="4.064" y1="1.524" x2="3.429" y2="1.524" width="0.1524" layer="21"/>
<wire x1="4.064" y1="-1.524" x2="3.429" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="4.318" y1="-1.27" x2="4.318" y2="1.27" width="0.1524" layer="21"/>
<pad name="1" x="-6.35" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="6.35" y="0" drill="0.8128" shape="octagon"/>
<text x="-4.191" y="1.905" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.175" y="-0.6858" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="4.318" y1="-0.3048" x2="5.1816" y2="0.3048" layer="21"/>
<rectangle x1="-5.1816" y1="-0.3048" x2="-4.318" y2="0.3048" layer="21"/>
</package>
<package name="0411/12">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0411, grid 12.5 mm</description>
<wire x1="6.35" y1="0" x2="5.461" y2="0" width="0.762" layer="51"/>
<wire x1="-6.35" y1="0" x2="-5.461" y2="0" width="0.762" layer="51"/>
<wire x1="5.08" y1="-1.651" x2="5.08" y2="1.651" width="0.1524" layer="21"/>
<wire x1="4.699" y1="2.032" x2="5.08" y2="1.651" width="0.1524" layer="21" curve="-90"/>
<wire x1="-5.08" y1="-1.651" x2="-4.699" y2="-2.032" width="0.1524" layer="21" curve="90"/>
<wire x1="4.699" y1="-2.032" x2="5.08" y2="-1.651" width="0.1524" layer="21" curve="90"/>
<wire x1="-5.08" y1="1.651" x2="-4.699" y2="2.032" width="0.1524" layer="21" curve="-90"/>
<wire x1="4.699" y1="2.032" x2="4.064" y2="2.032" width="0.1524" layer="21"/>
<wire x1="3.937" y1="1.905" x2="4.064" y2="2.032" width="0.1524" layer="21"/>
<wire x1="4.699" y1="-2.032" x2="4.064" y2="-2.032" width="0.1524" layer="21"/>
<wire x1="3.937" y1="-1.905" x2="4.064" y2="-2.032" width="0.1524" layer="21"/>
<wire x1="-3.937" y1="1.905" x2="-4.064" y2="2.032" width="0.1524" layer="21"/>
<wire x1="-3.937" y1="1.905" x2="3.937" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-3.937" y1="-1.905" x2="-4.064" y2="-2.032" width="0.1524" layer="21"/>
<wire x1="-3.937" y1="-1.905" x2="3.937" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="1.651" x2="-5.08" y2="-1.651" width="0.1524" layer="21"/>
<wire x1="-4.699" y1="2.032" x2="-4.064" y2="2.032" width="0.1524" layer="21"/>
<wire x1="-4.699" y1="-2.032" x2="-4.064" y2="-2.032" width="0.1524" layer="21"/>
<pad name="1" x="-6.35" y="0" drill="0.9144" shape="octagon"/>
<pad name="2" x="6.35" y="0" drill="0.9144" shape="octagon"/>
<text x="-5.08" y="2.413" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.5814" y="-0.635" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-5.3594" y1="-0.381" x2="-5.08" y2="0.381" layer="21"/>
<rectangle x1="5.08" y1="-0.381" x2="5.3594" y2="0.381" layer="21"/>
</package>
<package name="0411/15">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0411, grid 15 mm</description>
<wire x1="5.08" y1="-1.651" x2="5.08" y2="1.651" width="0.1524" layer="21"/>
<wire x1="4.699" y1="2.032" x2="5.08" y2="1.651" width="0.1524" layer="21" curve="-90"/>
<wire x1="-5.08" y1="-1.651" x2="-4.699" y2="-2.032" width="0.1524" layer="21" curve="90"/>
<wire x1="4.699" y1="-2.032" x2="5.08" y2="-1.651" width="0.1524" layer="21" curve="90"/>
<wire x1="-5.08" y1="1.651" x2="-4.699" y2="2.032" width="0.1524" layer="21" curve="-90"/>
<wire x1="4.699" y1="2.032" x2="4.064" y2="2.032" width="0.1524" layer="21"/>
<wire x1="3.937" y1="1.905" x2="4.064" y2="2.032" width="0.1524" layer="21"/>
<wire x1="4.699" y1="-2.032" x2="4.064" y2="-2.032" width="0.1524" layer="21"/>
<wire x1="3.937" y1="-1.905" x2="4.064" y2="-2.032" width="0.1524" layer="21"/>
<wire x1="-3.937" y1="1.905" x2="-4.064" y2="2.032" width="0.1524" layer="21"/>
<wire x1="-3.937" y1="1.905" x2="3.937" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-3.937" y1="-1.905" x2="-4.064" y2="-2.032" width="0.1524" layer="21"/>
<wire x1="-3.937" y1="-1.905" x2="3.937" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="1.651" x2="-5.08" y2="-1.651" width="0.1524" layer="21"/>
<wire x1="-4.699" y1="2.032" x2="-4.064" y2="2.032" width="0.1524" layer="21"/>
<wire x1="-4.699" y1="-2.032" x2="-4.064" y2="-2.032" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="0" x2="-6.35" y2="0" width="0.762" layer="51"/>
<wire x1="6.35" y1="0" x2="7.62" y2="0" width="0.762" layer="51"/>
<pad name="1" x="-7.62" y="0" drill="0.9144" shape="octagon"/>
<pad name="2" x="7.62" y="0" drill="0.9144" shape="octagon"/>
<text x="-5.08" y="2.413" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.5814" y="-0.635" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="5.08" y1="-0.381" x2="6.477" y2="0.381" layer="21"/>
<rectangle x1="-6.477" y1="-0.381" x2="-5.08" y2="0.381" layer="21"/>
</package>
<package name="0411V">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0411, grid 3.81 mm</description>
<wire x1="1.27" y1="0" x2="0.3048" y2="0" width="0.762" layer="51"/>
<wire x1="-1.5748" y1="0" x2="-2.54" y2="0" width="0.762" layer="51"/>
<circle x="-2.54" y="0" radius="2.032" width="0.1524" layer="21"/>
<circle x="-2.54" y="0" radius="1.016" width="0.1524" layer="51"/>
<pad name="1" x="-2.54" y="0" drill="0.9144" shape="octagon"/>
<pad name="2" x="1.27" y="0" drill="0.9144" shape="octagon"/>
<text x="-0.508" y="1.143" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-0.5334" y="-2.413" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-1.4732" y1="-0.381" x2="0.2032" y2="0.381" layer="21"/>
</package>
<package name="0414/15">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0414, grid 15 mm</description>
<wire x1="7.62" y1="0" x2="6.604" y2="0" width="0.8128" layer="51"/>
<wire x1="-7.62" y1="0" x2="-6.604" y2="0" width="0.8128" layer="51"/>
<wire x1="-6.096" y1="1.905" x2="-5.842" y2="2.159" width="0.1524" layer="21" curve="-90"/>
<wire x1="-6.096" y1="-1.905" x2="-5.842" y2="-2.159" width="0.1524" layer="21" curve="90"/>
<wire x1="5.842" y1="-2.159" x2="6.096" y2="-1.905" width="0.1524" layer="21" curve="90"/>
<wire x1="5.842" y1="2.159" x2="6.096" y2="1.905" width="0.1524" layer="21" curve="-90"/>
<wire x1="-6.096" y1="-1.905" x2="-6.096" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-5.842" y1="2.159" x2="-4.953" y2="2.159" width="0.1524" layer="21"/>
<wire x1="-4.826" y1="2.032" x2="-4.953" y2="2.159" width="0.1524" layer="21"/>
<wire x1="-5.842" y1="-2.159" x2="-4.953" y2="-2.159" width="0.1524" layer="21"/>
<wire x1="-4.826" y1="-2.032" x2="-4.953" y2="-2.159" width="0.1524" layer="21"/>
<wire x1="4.826" y1="2.032" x2="4.953" y2="2.159" width="0.1524" layer="21"/>
<wire x1="4.826" y1="2.032" x2="-4.826" y2="2.032" width="0.1524" layer="21"/>
<wire x1="4.826" y1="-2.032" x2="4.953" y2="-2.159" width="0.1524" layer="21"/>
<wire x1="4.826" y1="-2.032" x2="-4.826" y2="-2.032" width="0.1524" layer="21"/>
<wire x1="5.842" y1="2.159" x2="4.953" y2="2.159" width="0.1524" layer="21"/>
<wire x1="5.842" y1="-2.159" x2="4.953" y2="-2.159" width="0.1524" layer="21"/>
<wire x1="6.096" y1="-1.905" x2="6.096" y2="1.905" width="0.1524" layer="21"/>
<pad name="1" x="-7.62" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="7.62" y="0" drill="1.016" shape="octagon"/>
<text x="-6.096" y="2.5654" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-4.318" y="-0.5842" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="6.096" y1="-0.4064" x2="6.5024" y2="0.4064" layer="21"/>
<rectangle x1="-6.5024" y1="-0.4064" x2="-6.096" y2="0.4064" layer="21"/>
</package>
<package name="0414V">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0414, grid 5 mm</description>
<wire x1="2.54" y1="0" x2="1.397" y2="0" width="0.8128" layer="51"/>
<wire x1="-2.54" y1="0" x2="-1.397" y2="0" width="0.8128" layer="51"/>
<circle x="-2.54" y="0" radius="2.159" width="0.1524" layer="21"/>
<circle x="-2.54" y="0" radius="1.143" width="0.1524" layer="51"/>
<pad name="1" x="-2.54" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="2.54" y="0" drill="1.016" shape="octagon"/>
<text x="-0.381" y="1.1684" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-0.381" y="-2.3622" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-1.2954" y1="-0.4064" x2="1.2954" y2="0.4064" layer="21"/>
</package>
<package name="0617/17">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0617, grid 17.5 mm</description>
<wire x1="-8.89" y1="0" x2="-8.636" y2="0" width="0.8128" layer="51"/>
<wire x1="-7.874" y1="3.048" x2="-6.985" y2="3.048" width="0.1524" layer="21"/>
<wire x1="-6.731" y1="2.794" x2="-6.985" y2="3.048" width="0.1524" layer="21"/>
<wire x1="-7.874" y1="-3.048" x2="-6.985" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="-6.731" y1="-2.794" x2="-6.985" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="6.731" y1="2.794" x2="6.985" y2="3.048" width="0.1524" layer="21"/>
<wire x1="6.731" y1="2.794" x2="-6.731" y2="2.794" width="0.1524" layer="21"/>
<wire x1="6.731" y1="-2.794" x2="6.985" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="6.731" y1="-2.794" x2="-6.731" y2="-2.794" width="0.1524" layer="21"/>
<wire x1="7.874" y1="3.048" x2="6.985" y2="3.048" width="0.1524" layer="21"/>
<wire x1="7.874" y1="-3.048" x2="6.985" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="-8.255" y1="-2.667" x2="-8.255" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="-8.255" y1="1.016" x2="-8.255" y2="-1.016" width="0.1524" layer="51"/>
<wire x1="-8.255" y1="1.016" x2="-8.255" y2="2.667" width="0.1524" layer="21"/>
<wire x1="8.255" y1="-2.667" x2="8.255" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="8.255" y1="1.016" x2="8.255" y2="-1.016" width="0.1524" layer="51"/>
<wire x1="8.255" y1="1.016" x2="8.255" y2="2.667" width="0.1524" layer="21"/>
<wire x1="8.636" y1="0" x2="8.89" y2="0" width="0.8128" layer="51"/>
<wire x1="-8.255" y1="2.667" x2="-7.874" y2="3.048" width="0.1524" layer="21" curve="-90"/>
<wire x1="7.874" y1="3.048" x2="8.255" y2="2.667" width="0.1524" layer="21" curve="-90"/>
<wire x1="-8.255" y1="-2.667" x2="-7.874" y2="-3.048" width="0.1524" layer="21" curve="90"/>
<wire x1="7.874" y1="-3.048" x2="8.255" y2="-2.667" width="0.1524" layer="21" curve="90"/>
<pad name="1" x="-8.89" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="8.89" y="0" drill="1.016" shape="octagon"/>
<text x="-8.128" y="3.4544" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-6.096" y="-0.7112" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-8.5344" y1="-0.4064" x2="-8.2296" y2="0.4064" layer="51"/>
<rectangle x1="8.2296" y1="-0.4064" x2="8.5344" y2="0.4064" layer="51"/>
</package>
<package name="0617/22">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0617, grid 22.5 mm</description>
<wire x1="-10.287" y1="0" x2="-11.43" y2="0" width="0.8128" layer="51"/>
<wire x1="-8.255" y1="-2.667" x2="-8.255" y2="2.667" width="0.1524" layer="21"/>
<wire x1="-7.874" y1="3.048" x2="-6.985" y2="3.048" width="0.1524" layer="21"/>
<wire x1="-6.731" y1="2.794" x2="-6.985" y2="3.048" width="0.1524" layer="21"/>
<wire x1="-7.874" y1="-3.048" x2="-6.985" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="-6.731" y1="-2.794" x2="-6.985" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="6.731" y1="2.794" x2="6.985" y2="3.048" width="0.1524" layer="21"/>
<wire x1="6.731" y1="2.794" x2="-6.731" y2="2.794" width="0.1524" layer="21"/>
<wire x1="6.731" y1="-2.794" x2="6.985" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="6.731" y1="-2.794" x2="-6.731" y2="-2.794" width="0.1524" layer="21"/>
<wire x1="7.874" y1="3.048" x2="6.985" y2="3.048" width="0.1524" layer="21"/>
<wire x1="7.874" y1="-3.048" x2="6.985" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="8.255" y1="-2.667" x2="8.255" y2="2.667" width="0.1524" layer="21"/>
<wire x1="11.43" y1="0" x2="10.287" y2="0" width="0.8128" layer="51"/>
<wire x1="-8.255" y1="2.667" x2="-7.874" y2="3.048" width="0.1524" layer="21" curve="-90"/>
<wire x1="-8.255" y1="-2.667" x2="-7.874" y2="-3.048" width="0.1524" layer="21" curve="90"/>
<wire x1="7.874" y1="3.048" x2="8.255" y2="2.667" width="0.1524" layer="21" curve="-90"/>
<wire x1="7.874" y1="-3.048" x2="8.255" y2="-2.667" width="0.1524" layer="21" curve="90"/>
<pad name="1" x="-11.43" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="11.43" y="0" drill="1.016" shape="octagon"/>
<text x="-8.255" y="3.4544" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-6.477" y="-0.5842" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-10.1854" y1="-0.4064" x2="-8.255" y2="0.4064" layer="21"/>
<rectangle x1="8.255" y1="-0.4064" x2="10.1854" y2="0.4064" layer="21"/>
</package>
<package name="0617V">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0617, grid 5 mm</description>
<wire x1="-2.54" y1="0" x2="-1.27" y2="0" width="0.8128" layer="51"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.8128" layer="51"/>
<circle x="-2.54" y="0" radius="3.048" width="0.1524" layer="21"/>
<circle x="-2.54" y="0" radius="1.143" width="0.1524" layer="51"/>
<pad name="1" x="-2.54" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="2.54" y="0" drill="1.016" shape="octagon"/>
<text x="0.635" y="1.4224" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="0.635" y="-2.6162" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-1.3208" y1="-0.4064" x2="1.3208" y2="0.4064" layer="21"/>
</package>
<package name="0922/22">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0922, grid 22.5 mm</description>
<wire x1="11.43" y1="0" x2="10.795" y2="0" width="0.8128" layer="51"/>
<wire x1="-11.43" y1="0" x2="-10.795" y2="0" width="0.8128" layer="51"/>
<wire x1="-10.16" y1="-4.191" x2="-10.16" y2="4.191" width="0.1524" layer="21"/>
<wire x1="-9.779" y1="4.572" x2="-8.89" y2="4.572" width="0.1524" layer="21"/>
<wire x1="-8.636" y1="4.318" x2="-8.89" y2="4.572" width="0.1524" layer="21"/>
<wire x1="-9.779" y1="-4.572" x2="-8.89" y2="-4.572" width="0.1524" layer="21"/>
<wire x1="-8.636" y1="-4.318" x2="-8.89" y2="-4.572" width="0.1524" layer="21"/>
<wire x1="8.636" y1="4.318" x2="8.89" y2="4.572" width="0.1524" layer="21"/>
<wire x1="8.636" y1="4.318" x2="-8.636" y2="4.318" width="0.1524" layer="21"/>
<wire x1="8.636" y1="-4.318" x2="8.89" y2="-4.572" width="0.1524" layer="21"/>
<wire x1="8.636" y1="-4.318" x2="-8.636" y2="-4.318" width="0.1524" layer="21"/>
<wire x1="9.779" y1="4.572" x2="8.89" y2="4.572" width="0.1524" layer="21"/>
<wire x1="9.779" y1="-4.572" x2="8.89" y2="-4.572" width="0.1524" layer="21"/>
<wire x1="10.16" y1="-4.191" x2="10.16" y2="4.191" width="0.1524" layer="21"/>
<wire x1="-10.16" y1="-4.191" x2="-9.779" y2="-4.572" width="0.1524" layer="21" curve="90"/>
<wire x1="-10.16" y1="4.191" x2="-9.779" y2="4.572" width="0.1524" layer="21" curve="-90"/>
<wire x1="9.779" y1="-4.572" x2="10.16" y2="-4.191" width="0.1524" layer="21" curve="90"/>
<wire x1="9.779" y1="4.572" x2="10.16" y2="4.191" width="0.1524" layer="21" curve="-90"/>
<pad name="1" x="-11.43" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="11.43" y="0" drill="1.016" shape="octagon"/>
<text x="-10.16" y="5.1054" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-6.477" y="-0.5842" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-10.7188" y1="-0.4064" x2="-10.16" y2="0.4064" layer="51"/>
<rectangle x1="10.16" y1="-0.4064" x2="10.3124" y2="0.4064" layer="21"/>
<rectangle x1="-10.3124" y1="-0.4064" x2="-10.16" y2="0.4064" layer="21"/>
<rectangle x1="10.16" y1="-0.4064" x2="10.7188" y2="0.4064" layer="51"/>
</package>
<package name="P0613V">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0613, grid 5 mm</description>
<wire x1="2.54" y1="0" x2="1.397" y2="0" width="0.8128" layer="51"/>
<wire x1="-2.54" y1="0" x2="-1.397" y2="0" width="0.8128" layer="51"/>
<circle x="-2.54" y="0" radius="2.286" width="0.1524" layer="21"/>
<circle x="-2.54" y="0" radius="1.143" width="0.1524" layer="51"/>
<pad name="1" x="-2.54" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="2.54" y="0" drill="1.016" shape="octagon"/>
<text x="-0.254" y="1.143" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-0.254" y="-2.413" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-1.2954" y1="-0.4064" x2="1.3208" y2="0.4064" layer="21"/>
</package>
<package name="P0613/15">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0613, grid 15 mm</description>
<wire x1="7.62" y1="0" x2="6.985" y2="0" width="0.8128" layer="51"/>
<wire x1="-7.62" y1="0" x2="-6.985" y2="0" width="0.8128" layer="51"/>
<wire x1="-6.477" y1="2.032" x2="-6.223" y2="2.286" width="0.1524" layer="21" curve="-90"/>
<wire x1="-6.477" y1="-2.032" x2="-6.223" y2="-2.286" width="0.1524" layer="21" curve="90"/>
<wire x1="6.223" y1="-2.286" x2="6.477" y2="-2.032" width="0.1524" layer="21" curve="90"/>
<wire x1="6.223" y1="2.286" x2="6.477" y2="2.032" width="0.1524" layer="21" curve="-90"/>
<wire x1="-6.223" y1="2.286" x2="-5.334" y2="2.286" width="0.1524" layer="21"/>
<wire x1="-5.207" y1="2.159" x2="-5.334" y2="2.286" width="0.1524" layer="21"/>
<wire x1="-6.223" y1="-2.286" x2="-5.334" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="-5.207" y1="-2.159" x2="-5.334" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="5.207" y1="2.159" x2="5.334" y2="2.286" width="0.1524" layer="21"/>
<wire x1="5.207" y1="2.159" x2="-5.207" y2="2.159" width="0.1524" layer="21"/>
<wire x1="5.207" y1="-2.159" x2="5.334" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="5.207" y1="-2.159" x2="-5.207" y2="-2.159" width="0.1524" layer="21"/>
<wire x1="6.223" y1="2.286" x2="5.334" y2="2.286" width="0.1524" layer="21"/>
<wire x1="6.223" y1="-2.286" x2="5.334" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="6.477" y1="-0.635" x2="6.477" y2="-2.032" width="0.1524" layer="21"/>
<wire x1="6.477" y1="-0.635" x2="6.477" y2="0.635" width="0.1524" layer="51"/>
<wire x1="6.477" y1="2.032" x2="6.477" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-6.477" y1="-2.032" x2="-6.477" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-6.477" y1="0.635" x2="-6.477" y2="-0.635" width="0.1524" layer="51"/>
<wire x1="-6.477" y1="0.635" x2="-6.477" y2="2.032" width="0.1524" layer="21"/>
<pad name="1" x="-7.62" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="7.62" y="0" drill="1.016" shape="octagon"/>
<text x="-6.477" y="2.6924" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-4.318" y="-0.7112" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-7.0358" y1="-0.4064" x2="-6.477" y2="0.4064" layer="51"/>
<rectangle x1="6.477" y1="-0.4064" x2="7.0358" y2="0.4064" layer="51"/>
</package>
<package name="P0817/22">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0817, grid 22.5 mm</description>
<wire x1="-10.414" y1="0" x2="-11.43" y2="0" width="0.8128" layer="51"/>
<wire x1="-8.509" y1="-3.429" x2="-8.509" y2="3.429" width="0.1524" layer="21"/>
<wire x1="-8.128" y1="3.81" x2="-7.239" y2="3.81" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="3.556" x2="-7.239" y2="3.81" width="0.1524" layer="21"/>
<wire x1="-8.128" y1="-3.81" x2="-7.239" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="-3.556" x2="-7.239" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="6.985" y1="3.556" x2="7.239" y2="3.81" width="0.1524" layer="21"/>
<wire x1="6.985" y1="3.556" x2="-6.985" y2="3.556" width="0.1524" layer="21"/>
<wire x1="6.985" y1="-3.556" x2="7.239" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="6.985" y1="-3.556" x2="-6.985" y2="-3.556" width="0.1524" layer="21"/>
<wire x1="8.128" y1="3.81" x2="7.239" y2="3.81" width="0.1524" layer="21"/>
<wire x1="8.128" y1="-3.81" x2="7.239" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="8.509" y1="-3.429" x2="8.509" y2="3.429" width="0.1524" layer="21"/>
<wire x1="11.43" y1="0" x2="10.414" y2="0" width="0.8128" layer="51"/>
<wire x1="-8.509" y1="3.429" x2="-8.128" y2="3.81" width="0.1524" layer="21" curve="-90"/>
<wire x1="-8.509" y1="-3.429" x2="-8.128" y2="-3.81" width="0.1524" layer="21" curve="90"/>
<wire x1="8.128" y1="3.81" x2="8.509" y2="3.429" width="0.1524" layer="21" curve="-90"/>
<wire x1="8.128" y1="-3.81" x2="8.509" y2="-3.429" width="0.1524" layer="21" curve="90"/>
<pad name="1" x="-11.43" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="11.43" y="0" drill="1.016" shape="octagon"/>
<text x="-8.382" y="4.2164" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-6.223" y="-0.5842" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<text x="6.604" y="-2.2606" size="1.27" layer="51" ratio="10" rot="R90">0817</text>
<rectangle x1="8.509" y1="-0.4064" x2="10.3124" y2="0.4064" layer="21"/>
<rectangle x1="-10.3124" y1="-0.4064" x2="-8.509" y2="0.4064" layer="21"/>
</package>
<package name="P0817V">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0817, grid 6.35 mm</description>
<wire x1="-3.81" y1="0" x2="-5.08" y2="0" width="0.8128" layer="51"/>
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.8128" layer="51"/>
<circle x="-5.08" y="0" radius="3.81" width="0.1524" layer="21"/>
<circle x="-5.08" y="0" radius="1.27" width="0.1524" layer="51"/>
<pad name="1" x="-5.08" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="1.27" y="0" drill="1.016" shape="octagon"/>
<text x="-1.016" y="1.27" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.016" y="-2.54" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<text x="-6.858" y="2.032" size="1.016" layer="21" ratio="12">0817</text>
<rectangle x1="-3.81" y1="-0.4064" x2="0" y2="0.4064" layer="21"/>
</package>
<package name="V234/12">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type V234, grid 12.5 mm</description>
<wire x1="-4.953" y1="1.524" x2="-4.699" y2="1.778" width="0.1524" layer="21" curve="-90"/>
<wire x1="4.699" y1="1.778" x2="4.953" y2="1.524" width="0.1524" layer="21" curve="-90"/>
<wire x1="4.699" y1="-1.778" x2="4.953" y2="-1.524" width="0.1524" layer="21" curve="90"/>
<wire x1="-4.953" y1="-1.524" x2="-4.699" y2="-1.778" width="0.1524" layer="21" curve="90"/>
<wire x1="-4.699" y1="1.778" x2="4.699" y2="1.778" width="0.1524" layer="21"/>
<wire x1="-4.953" y1="1.524" x2="-4.953" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="4.699" y1="-1.778" x2="-4.699" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="4.953" y1="1.524" x2="4.953" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="6.35" y1="0" x2="5.461" y2="0" width="0.8128" layer="51"/>
<wire x1="-6.35" y1="0" x2="-5.461" y2="0" width="0.8128" layer="51"/>
<pad name="1" x="-6.35" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="6.35" y="0" drill="1.016" shape="octagon"/>
<text x="-4.953" y="2.159" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.81" y="-0.635" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="4.953" y1="-0.4064" x2="5.4102" y2="0.4064" layer="21"/>
<rectangle x1="-5.4102" y1="-0.4064" x2="-4.953" y2="0.4064" layer="21"/>
</package>
<package name="V235/17">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type V235, grid 17.78 mm</description>
<wire x1="-6.731" y1="2.921" x2="6.731" y2="2.921" width="0.1524" layer="21"/>
<wire x1="-7.112" y1="2.54" x2="-7.112" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="6.731" y1="-2.921" x2="-6.731" y2="-2.921" width="0.1524" layer="21"/>
<wire x1="7.112" y1="2.54" x2="7.112" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="8.89" y1="0" x2="7.874" y2="0" width="1.016" layer="51"/>
<wire x1="-7.874" y1="0" x2="-8.89" y2="0" width="1.016" layer="51"/>
<wire x1="-7.112" y1="-2.54" x2="-6.731" y2="-2.921" width="0.1524" layer="21" curve="90"/>
<wire x1="6.731" y1="2.921" x2="7.112" y2="2.54" width="0.1524" layer="21" curve="-90"/>
<wire x1="6.731" y1="-2.921" x2="7.112" y2="-2.54" width="0.1524" layer="21" curve="90"/>
<wire x1="-7.112" y1="2.54" x2="-6.731" y2="2.921" width="0.1524" layer="21" curve="-90"/>
<pad name="1" x="-8.89" y="0" drill="1.1938" shape="octagon"/>
<pad name="2" x="8.89" y="0" drill="1.1938" shape="octagon"/>
<text x="-6.858" y="3.302" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-5.842" y="-0.635" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="7.112" y1="-0.508" x2="7.747" y2="0.508" layer="21"/>
<rectangle x1="-7.747" y1="-0.508" x2="-7.112" y2="0.508" layer="21"/>
</package>
<package name="V526-0">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type V526-0, grid 2.5 mm</description>
<wire x1="-2.54" y1="1.016" x2="-2.286" y2="1.27" width="0.1524" layer="21" curve="-90"/>
<wire x1="2.286" y1="1.27" x2="2.54" y2="1.016" width="0.1524" layer="21" curve="-90"/>
<wire x1="2.286" y1="-1.27" x2="2.54" y2="-1.016" width="0.1524" layer="21" curve="90"/>
<wire x1="-2.54" y1="-1.016" x2="-2.286" y2="-1.27" width="0.1524" layer="21" curve="90"/>
<wire x1="2.286" y1="1.27" x2="-2.286" y2="1.27" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-1.016" x2="2.54" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-2.286" y1="-1.27" x2="2.286" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="1.016" x2="-2.54" y2="-1.016" width="0.1524" layer="21"/>
<pad name="1" x="-1.27" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="1.27" y="0" drill="0.8128" shape="octagon"/>
<text x="-2.413" y="1.651" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.413" y="-2.794" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="MINI_MELF-0102AX">
<description>&lt;b&gt;Mini MELF 0102 Axial&lt;/b&gt;</description>
<circle x="0" y="0" radius="0.6" width="0" layer="51"/>
<circle x="0" y="0" radius="0.6" width="0" layer="52"/>
<smd name="1" x="0" y="0" dx="1.9" dy="1.9" layer="1" roundness="100"/>
<smd name="2" x="0" y="0" dx="1.9" dy="1.9" layer="16" roundness="100"/>
<text x="-1.27" y="0.9525" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.27" y="-2.2225" size="1.27" layer="27">&gt;VALUE</text>
<hole x="0" y="0" drill="1.3"/>
</package>
<package name="0922V">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0922, grid 7.5 mm</description>
<wire x1="2.54" y1="0" x2="1.397" y2="0" width="0.8128" layer="51"/>
<wire x1="-5.08" y1="0" x2="-3.81" y2="0" width="0.8128" layer="51"/>
<circle x="-5.08" y="0" radius="4.572" width="0.1524" layer="21"/>
<circle x="-5.08" y="0" radius="1.905" width="0.1524" layer="21"/>
<pad name="1" x="-5.08" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="2.54" y="0" drill="1.016" shape="octagon"/>
<text x="-0.508" y="1.6764" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-0.508" y="-2.9972" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<text x="-6.858" y="2.54" size="1.016" layer="21" ratio="12">0922</text>
<rectangle x1="-3.81" y1="-0.4064" x2="1.3208" y2="0.4064" layer="21"/>
</package>
<package name="MINI_MELF-0102R">
<description>&lt;b&gt;CECC Size RC2211&lt;/b&gt; Reflow Soldering&lt;p&gt;
source Beyschlag</description>
<wire x1="-1" y1="-0.5" x2="1" y2="-0.5" width="0.2032" layer="51"/>
<wire x1="1" y1="-0.5" x2="1" y2="0.5" width="0.2032" layer="51"/>
<wire x1="1" y1="0.5" x2="-1" y2="0.5" width="0.2032" layer="51"/>
<wire x1="-1" y1="0.5" x2="-1" y2="-0.5" width="0.2032" layer="51"/>
<smd name="1" x="-0.9" y="0" dx="0.5" dy="1.3" layer="1"/>
<smd name="2" x="0.9" y="0" dx="0.5" dy="1.3" layer="1"/>
<text x="-1.27" y="0.9525" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.27" y="-2.2225" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="MINI_MELF-0102W">
<description>&lt;b&gt;CECC Size RC2211&lt;/b&gt; Wave Soldering&lt;p&gt;
source Beyschlag</description>
<wire x1="-1" y1="-0.5" x2="1" y2="-0.5" width="0.2032" layer="51"/>
<wire x1="1" y1="-0.5" x2="1" y2="0.5" width="0.2032" layer="51"/>
<wire x1="1" y1="0.5" x2="-1" y2="0.5" width="0.2032" layer="51"/>
<wire x1="-1" y1="0.5" x2="-1" y2="-0.5" width="0.2032" layer="51"/>
<smd name="1" x="-0.95" y="0" dx="0.6" dy="1.3" layer="1"/>
<smd name="2" x="0.95" y="0" dx="0.6" dy="1.3" layer="1"/>
<text x="-1.27" y="0.9525" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.27" y="-2.2225" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="MINI_MELF-0204R">
<description>&lt;b&gt;CECC Size RC3715&lt;/b&gt; Reflow Soldering&lt;p&gt;
source Beyschlag</description>
<wire x1="-1.7" y1="-0.6" x2="1.7" y2="-0.6" width="0.2032" layer="51"/>
<wire x1="1.7" y1="-0.6" x2="1.7" y2="0.6" width="0.2032" layer="51"/>
<wire x1="1.7" y1="0.6" x2="-1.7" y2="0.6" width="0.2032" layer="51"/>
<wire x1="-1.7" y1="0.6" x2="-1.7" y2="-0.6" width="0.2032" layer="51"/>
<smd name="1" x="-1.5" y="0" dx="0.8" dy="1.6" layer="1"/>
<smd name="2" x="1.5" y="0" dx="0.8" dy="1.6" layer="1"/>
<text x="-1.27" y="0.9525" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.27" y="-2.2225" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="MINI_MELF-0204W">
<description>&lt;b&gt;CECC Size RC3715&lt;/b&gt; Wave Soldering&lt;p&gt;
source Beyschlag</description>
<wire x1="-1.7" y1="-0.6" x2="1.7" y2="-0.6" width="0.2032" layer="51"/>
<wire x1="1.7" y1="-0.6" x2="1.7" y2="0.6" width="0.2032" layer="51"/>
<wire x1="1.7" y1="0.6" x2="-1.7" y2="0.6" width="0.2032" layer="51"/>
<wire x1="-1.7" y1="0.6" x2="-1.7" y2="-0.6" width="0.2032" layer="51"/>
<smd name="1" x="-1.5" y="0" dx="1.2" dy="1.6" layer="1"/>
<smd name="2" x="1.5" y="0" dx="1.2" dy="1.6" layer="1"/>
<text x="-1.27" y="0.9525" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.27" y="-2.2225" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="MINI_MELF-0207R">
<description>&lt;b&gt;CECC Size RC6123&lt;/b&gt; Reflow Soldering&lt;p&gt;
source Beyschlag</description>
<wire x1="-2.8" y1="-1" x2="2.8" y2="-1" width="0.2032" layer="51"/>
<wire x1="2.8" y1="-1" x2="2.8" y2="1" width="0.2032" layer="51"/>
<wire x1="2.8" y1="1" x2="-2.8" y2="1" width="0.2032" layer="51"/>
<wire x1="-2.8" y1="1" x2="-2.8" y2="-1" width="0.2032" layer="51"/>
<smd name="1" x="-2.25" y="0" dx="1.6" dy="2.5" layer="1"/>
<smd name="2" x="2.25" y="0" dx="1.6" dy="2.5" layer="1"/>
<text x="-2.2225" y="1.5875" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.2225" y="-2.54" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="MINI_MELF-0207W">
<description>&lt;b&gt;CECC Size RC6123&lt;/b&gt; Wave Soldering&lt;p&gt;
source Beyschlag</description>
<wire x1="-2.8" y1="-1" x2="2.8" y2="-1" width="0.2032" layer="51"/>
<wire x1="2.8" y1="-1" x2="2.8" y2="1" width="0.2032" layer="51"/>
<wire x1="2.8" y1="1" x2="-2.8" y2="1" width="0.2032" layer="51"/>
<wire x1="-2.8" y1="1" x2="-2.8" y2="-1" width="0.2032" layer="51"/>
<smd name="1" x="-2.6" y="0" dx="2.4" dy="2.5" layer="1"/>
<smd name="2" x="2.6" y="0" dx="2.4" dy="2.5" layer="1"/>
<text x="-2.54" y="1.5875" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.54" y="-2.54" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="RDH/15">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type RDH, grid 15 mm</description>
<wire x1="-7.62" y1="0" x2="-6.858" y2="0" width="0.8128" layer="51"/>
<wire x1="-6.096" y1="3.048" x2="-5.207" y2="3.048" width="0.1524" layer="21"/>
<wire x1="-4.953" y1="2.794" x2="-5.207" y2="3.048" width="0.1524" layer="21"/>
<wire x1="-6.096" y1="-3.048" x2="-5.207" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="-4.953" y1="-2.794" x2="-5.207" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="4.953" y1="2.794" x2="5.207" y2="3.048" width="0.1524" layer="21"/>
<wire x1="4.953" y1="2.794" x2="-4.953" y2="2.794" width="0.1524" layer="21"/>
<wire x1="4.953" y1="-2.794" x2="5.207" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="4.953" y1="-2.794" x2="-4.953" y2="-2.794" width="0.1524" layer="21"/>
<wire x1="6.096" y1="3.048" x2="5.207" y2="3.048" width="0.1524" layer="21"/>
<wire x1="6.096" y1="-3.048" x2="5.207" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="-6.477" y1="-2.667" x2="-6.477" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="-6.477" y1="1.016" x2="-6.477" y2="-1.016" width="0.1524" layer="51"/>
<wire x1="-6.477" y1="1.016" x2="-6.477" y2="2.667" width="0.1524" layer="21"/>
<wire x1="6.477" y1="-2.667" x2="6.477" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="6.477" y1="1.016" x2="6.477" y2="-1.016" width="0.1524" layer="51"/>
<wire x1="6.477" y1="1.016" x2="6.477" y2="2.667" width="0.1524" layer="21"/>
<wire x1="6.858" y1="0" x2="7.62" y2="0" width="0.8128" layer="51"/>
<wire x1="-6.477" y1="2.667" x2="-6.096" y2="3.048" width="0.1524" layer="21" curve="-90"/>
<wire x1="6.096" y1="3.048" x2="6.477" y2="2.667" width="0.1524" layer="21" curve="-90"/>
<wire x1="-6.477" y1="-2.667" x2="-6.096" y2="-3.048" width="0.1524" layer="21" curve="90"/>
<wire x1="6.096" y1="-3.048" x2="6.477" y2="-2.667" width="0.1524" layer="21" curve="90"/>
<pad name="1" x="-7.62" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="7.62" y="0" drill="1.016" shape="octagon"/>
<text x="-6.35" y="3.4544" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-4.318" y="-0.5842" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<text x="4.572" y="-1.7272" size="1.27" layer="51" ratio="10" rot="R90">RDH</text>
<rectangle x1="-6.7564" y1="-0.4064" x2="-6.4516" y2="0.4064" layer="51"/>
<rectangle x1="6.4516" y1="-0.4064" x2="6.7564" y2="0.4064" layer="51"/>
</package>
<package name="0204V">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0204, grid 2.5 mm</description>
<wire x1="-1.27" y1="0" x2="1.27" y2="0" width="0.508" layer="51"/>
<wire x1="-0.127" y1="0" x2="0.127" y2="0" width="0.508" layer="21"/>
<circle x="-1.27" y="0" radius="0.889" width="0.1524" layer="51"/>
<circle x="-1.27" y="0" radius="0.635" width="0.0508" layer="51"/>
<pad name="1" x="-1.27" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="1.27" y="0" drill="0.8128" shape="octagon"/>
<text x="-2.1336" y="1.1684" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.1336" y="-2.3114" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="0309V">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
type 0309, grid 2.5 mm</description>
<wire x1="1.27" y1="0" x2="0.635" y2="0" width="0.6096" layer="51"/>
<wire x1="-0.635" y1="0" x2="-1.27" y2="0" width="0.6096" layer="51"/>
<circle x="-1.27" y="0" radius="1.524" width="0.1524" layer="21"/>
<circle x="-1.27" y="0" radius="0.762" width="0.1524" layer="51"/>
<pad name="1" x="-1.27" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="1.27" y="0" drill="0.8128" shape="octagon"/>
<text x="0.254" y="1.016" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="0.254" y="-2.2098" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="0.254" y1="-0.3048" x2="0.5588" y2="0.3048" layer="51"/>
<rectangle x1="-0.635" y1="-0.3048" x2="-0.3302" y2="0.3048" layer="51"/>
<rectangle x1="-0.3302" y1="-0.3048" x2="0.254" y2="0.3048" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="R-US">
<wire x1="-2.54" y1="0" x2="-2.159" y2="1.016" width="0.2032" layer="94"/>
<wire x1="-2.159" y1="1.016" x2="-1.524" y2="-1.016" width="0.2032" layer="94"/>
<wire x1="-1.524" y1="-1.016" x2="-0.889" y2="1.016" width="0.2032" layer="94"/>
<wire x1="-0.889" y1="1.016" x2="-0.254" y2="-1.016" width="0.2032" layer="94"/>
<wire x1="-0.254" y1="-1.016" x2="0.381" y2="1.016" width="0.2032" layer="94"/>
<wire x1="0.381" y1="1.016" x2="1.016" y2="-1.016" width="0.2032" layer="94"/>
<wire x1="1.016" y1="-1.016" x2="1.651" y2="1.016" width="0.2032" layer="94"/>
<wire x1="1.651" y1="1.016" x2="2.286" y2="-1.016" width="0.2032" layer="94"/>
<wire x1="2.286" y1="-1.016" x2="2.54" y2="0" width="0.2032" layer="94"/>
<text x="-3.81" y="1.4986" size="1.778" layer="95">&gt;NAME</text>
<text x="-3.81" y="-3.302" size="1.778" layer="96">&gt;VALUE</text>
<pin name="2" x="5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1" rot="R180"/>
<pin name="1" x="-5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="R-US_" prefix="R" uservalue="yes">
<description>&lt;B&gt;RESISTOR&lt;/B&gt;, American symbol</description>
<gates>
<gate name="G$1" symbol="R-US" x="0" y="0"/>
</gates>
<devices>
<device name="R0402" package="R0402">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R0603" package="R0603">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R0805" package="R0805">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R0805W" package="R0805W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R1005" package="R1005">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R1206" package="R1206">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R1206W" package="R1206W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R1210" package="R1210">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R1210W" package="R1210W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R2010" package="R2010">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R2010W" package="R2010W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R2012" package="R2012">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R2012W" package="R2012W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R2512" package="R2512">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R2512W" package="R2512W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R3216" package="R3216">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R3216W" package="R3216W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R3225" package="R3225">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R3225W" package="R3225W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R5025" package="R5025">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R5025W" package="R5025W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R6332" package="R6332">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R6332W" package="R6332W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="M0805" package="M0805">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="M1206" package="M1206">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="M1406" package="M1406">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="M2012" package="M2012">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="M2309" package="M2309">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="M3216" package="M3216">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="M3516" package="M3516">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="M5923" package="M5923">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0204/5" package="0204/5">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0204/7" package="0204/7">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0207/10" package="0207/10">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0207/12" package="0207/12">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0207/15" package="0207/15">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0207/2V" package="0207/2V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0207/5V" package="0207/5V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0207/7" package="0207/7">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0309/10" package="0309/10">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0309/12" package="0309/12">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0411/12" package="0411/12">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0411/15" package="0411/15">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0411/3V" package="0411V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0414/15" package="0414/15">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0414/5V" package="0414V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0617/17" package="0617/17">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0617/22" package="0617/22">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0617/5V" package="0617V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0922/22" package="0922/22">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0613/5V" package="P0613V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0613/15" package="P0613/15">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0817/22" package="P0817/22">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0817/7V" package="P0817V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="V234/12" package="V234/12">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="V235/17" package="V235/17">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="V526-0" package="V526-0">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="MELF0102AX" package="MINI_MELF-0102AX">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0922V" package="0922V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="MELF0102R" package="MINI_MELF-0102R">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="MELF0102W" package="MINI_MELF-0102W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="MELF0204R" package="MINI_MELF-0204R">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="MELF0204W" package="MINI_MELF-0204W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="MELF0207R" package="MINI_MELF-0207R">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="MELF0207W" package="MINI_MELF-0207W">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="RDH/15" package="RDH/15">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0204/2V" package="0204V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="0309/V" package="0309V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="con-molex">
<description>&lt;b&gt;Molex Connectors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="22-23-2021">
<description>.100" (2.54mm) Center Headers - 2 Pin</description>
<wire x1="-2.54" y1="3.175" x2="2.54" y2="3.175" width="0.254" layer="21"/>
<wire x1="2.54" y1="3.175" x2="2.54" y2="1.27" width="0.254" layer="21"/>
<wire x1="2.54" y1="1.27" x2="2.54" y2="-3.175" width="0.254" layer="21"/>
<wire x1="2.54" y1="-3.175" x2="-2.54" y2="-3.175" width="0.254" layer="21"/>
<wire x1="-2.54" y1="-3.175" x2="-2.54" y2="1.27" width="0.254" layer="21"/>
<wire x1="-2.54" y1="1.27" x2="-2.54" y2="3.175" width="0.254" layer="21"/>
<wire x1="-2.54" y1="1.27" x2="2.54" y2="1.27" width="0.254" layer="21"/>
<pad name="1" x="-1.27" y="0" drill="1" shape="long" rot="R90"/>
<pad name="2" x="1.27" y="0" drill="1" shape="long" rot="R90"/>
<text x="-2.54" y="3.81" size="1.016" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="-5.08" size="1.016" layer="27" ratio="10">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="MV">
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<text x="-0.762" y="1.397" size="1.778" layer="96">&gt;VALUE</text>
<pin name="S" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
<symbol name="M">
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<pin name="S" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="22-23-2021" prefix="X">
<description>.100" (2.54mm) Center Header - 2 Pin</description>
<gates>
<gate name="-1" symbol="MV" x="0" y="0" addlevel="always" swaplevel="1"/>
<gate name="-2" symbol="M" x="0" y="-2.54" addlevel="always" swaplevel="1"/>
</gates>
<devices>
<device name="" package="22-23-2021">
<connects>
<connect gate="-1" pin="S" pad="1"/>
<connect gate="-2" pin="S" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="MOLEX" constant="no"/>
<attribute name="MPN" value="22-23-2021" constant="no"/>
<attribute name="OC_FARNELL" value="1462926" constant="no"/>
<attribute name="OC_NEWARK" value="25C3832" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
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
<package name="M25D">
<description>&lt;b&gt;SUB-D&lt;/b&gt;</description>
<wire x1="-19.812" y1="6.745" x2="-19.304" y2="7.253" width="0.1524" layer="21" curve="-90"/>
<wire x1="19.304" y1="7.253" x2="19.812" y2="6.745" width="0.1524" layer="21" curve="-90"/>
<wire x1="19.812" y1="2.046" x2="19.812" y2="6.745" width="0.1524" layer="21"/>
<wire x1="19.812" y1="2.046" x2="20.447" y2="1.411" width="0.1524" layer="21" curve="90"/>
<wire x1="-19.812" y1="2.046" x2="-19.812" y2="6.745" width="0.1524" layer="21"/>
<wire x1="-20.447" y1="1.411" x2="-19.812" y2="2.046" width="0.1524" layer="21" curve="90"/>
<wire x1="-19.304" y1="7.253" x2="19.304" y2="7.253" width="0.1524" layer="21"/>
<wire x1="-24.6888" y1="-2.7432" x2="-23.3172" y2="-4.1148" width="0.254" layer="48"/>
<wire x1="-23.3172" y1="-4.1148" x2="-21.9456" y2="-2.7432" width="0.254" layer="48"/>
<wire x1="-21.9456" y1="-2.7432" x2="-20.574" y2="-4.1148" width="0.254" layer="48"/>
<wire x1="-20.574" y1="-4.1148" x2="-19.2024" y2="-2.7432" width="0.254" layer="48"/>
<wire x1="19.2024" y1="-2.7432" x2="20.574" y2="-4.1148" width="0.254" layer="48"/>
<wire x1="20.574" y1="-4.1148" x2="21.9456" y2="-2.7432" width="0.254" layer="48"/>
<wire x1="21.9456" y1="-2.7432" x2="23.3172" y2="-4.1148" width="0.254" layer="48"/>
<wire x1="23.3172" y1="-4.1148" x2="24.6888" y2="-2.7432" width="0.254" layer="48"/>
<wire x1="5.08" y1="3.669" x2="2.794" y2="3.669" width="0.1524" layer="21"/>
<wire x1="2.794" y1="3.669" x2="2.794" y2="5.955" width="0.1524" layer="21"/>
<wire x1="5.08" y1="5.955" x2="5.08" y2="3.669" width="0.1524" layer="21"/>
<wire x1="1.143" y1="3.669" x2="-1.143" y2="3.669" width="0.1524" layer="21"/>
<wire x1="-1.143" y1="3.669" x2="-1.143" y2="5.955" width="0.1524" layer="21"/>
<wire x1="1.143" y1="5.955" x2="1.143" y2="3.669" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="3.669" x2="-5.08" y2="3.669" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="3.669" x2="-5.08" y2="5.955" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="5.955" x2="-2.794" y2="3.669" width="0.1524" layer="21"/>
<wire x1="-18.6436" y1="0.508" x2="-18.6436" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-18.6436" y1="0.508" x2="-18.5166" y2="0.508" width="0.1524" layer="21"/>
<wire x1="-18.5166" y1="-1.905" x2="-17.8816" y2="-2.54" width="0.1524" layer="21" curve="90"/>
<wire x1="-18.5166" y1="-1.905" x2="-18.5166" y2="0.508" width="0.1524" layer="21"/>
<wire x1="-17.901" y1="-2.54" x2="17.901" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="18.6436" y1="0.508" x2="18.6436" y2="1.016" width="0.1524" layer="21"/>
<wire x1="18.5166" y1="-1.905" x2="18.5166" y2="0.508" width="0.1524" layer="21"/>
<wire x1="18.5166" y1="0.508" x2="18.6436" y2="0.508" width="0.1524" layer="21"/>
<wire x1="17.8816" y1="-2.54" x2="18.5166" y2="-1.905" width="0.1524" layer="21" curve="90"/>
<smd name="1" x="16.4594" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="2" x="13.716" y="-6.045" dx="1.7" dy="3.5" layer="1"/>
<smd name="3" x="10.9726" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="4" x="8.2298" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="5" x="5.4862" y="-6.0446" dx="1.7" dy="3.5" layer="1"/>
<smd name="6" x="2.7434" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="7" x="0" y="-6.045" dx="1.7" dy="3.5" layer="1"/>
<smd name="8" x="-2.7434" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="9" x="-5.4862" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="10" x="-8.2298" y="-6.0446" dx="1.7" dy="3.5" layer="1"/>
<smd name="11" x="-10.9726" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="12" x="-13.716" y="-6.045" dx="1.7" dy="3.5" layer="1"/>
<smd name="13" x="-16.4594" y="-6.0448" dx="1.7" dy="3.5" layer="1"/>
<smd name="14" x="15.0876" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="15" x="12.3444" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="16" x="9.6012" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="17" x="6.858" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="18" x="4.1148" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="19" x="1.3716" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="20" x="-1.3716" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="21" x="-4.1148" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="22" x="-6.858" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="23" x="-9.6012" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="24" x="-12.3444" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<smd name="25" x="-15.0876" y="-6.0452" dx="1.7" dy="3.5" layer="16"/>
<text x="-10.2616" y="-10.16" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-0.1016" y="-10.16" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="16.0256" y="-2.1786" size="1.27" layer="21" ratio="10">1</text>
<text x="19.6596" y="-2.54" size="1.27" layer="48">Board</text>
<text x="-14.6606" y="-0.825" size="1.27" layer="22" ratio="10" rot="MR0">25</text>
<text x="-16.8606" y="-2.1786" size="1.27" layer="21" ratio="10">13</text>
<text x="15.6256" y="-0.825" size="1.27" layer="22" ratio="10" rot="MR0">14</text>
<text x="-17.2212" y="4.5974" size="1.27" layer="51" ratio="10">M25D</text>
<text x="-24.7396" y="-2.54" size="1.27" layer="48">Board </text>
<rectangle x1="14.4876" y1="-6.56" x2="15.6876" y2="-4.06" layer="52"/>
<rectangle x1="15.8592" y1="-6.56" x2="17.0592" y2="-4.06" layer="51"/>
<rectangle x1="11.7444" y1="-6.56" x2="12.9444" y2="-4.06" layer="52"/>
<rectangle x1="13.116" y1="-6.56" x2="14.316" y2="-4.06" layer="51"/>
<rectangle x1="9.0012" y1="-6.56" x2="10.2012" y2="-4.06" layer="52"/>
<rectangle x1="10.3728" y1="-6.56" x2="11.5728" y2="-4.06" layer="51"/>
<rectangle x1="6.258" y1="-6.56" x2="7.458" y2="-4.06" layer="52"/>
<rectangle x1="7.6296" y1="-6.56" x2="8.8296" y2="-4.06" layer="51"/>
<rectangle x1="3.5148" y1="-6.56" x2="4.7148" y2="-4.06" layer="52"/>
<rectangle x1="4.8864" y1="-6.56" x2="6.0864" y2="-4.06" layer="51"/>
<rectangle x1="0.7716" y1="-6.56" x2="1.9716" y2="-4.06" layer="52"/>
<rectangle x1="2.1432" y1="-6.56" x2="3.3432" y2="-4.06" layer="51"/>
<rectangle x1="-1.9716" y1="-6.56" x2="-0.7716" y2="-4.06" layer="52"/>
<rectangle x1="-0.6" y1="-6.56" x2="0.6" y2="-4.06" layer="51"/>
<rectangle x1="-3.3432" y1="-6.56" x2="-2.1432" y2="-4.06" layer="51"/>
<rectangle x1="-4.7148" y1="-6.56" x2="-3.5148" y2="-4.06" layer="52"/>
<rectangle x1="-7.458" y1="-6.56" x2="-6.258" y2="-4.06" layer="52"/>
<rectangle x1="-6.0864" y1="-6.56" x2="-4.8864" y2="-4.06" layer="51"/>
<rectangle x1="-10.2012" y1="-6.56" x2="-9.0012" y2="-4.06" layer="52"/>
<rectangle x1="-8.8296" y1="-6.56" x2="-7.6296" y2="-4.06" layer="51"/>
<rectangle x1="-12.9444" y1="-6.56" x2="-11.7444" y2="-4.06" layer="52"/>
<rectangle x1="-11.5728" y1="-6.56" x2="-10.3728" y2="-4.06" layer="51"/>
<rectangle x1="-15.6876" y1="-6.56" x2="-14.4876" y2="-4.06" layer="52"/>
<rectangle x1="-14.316" y1="-6.56" x2="-13.116" y2="-4.06" layer="51"/>
<rectangle x1="-17.0592" y1="-6.56" x2="-15.8592" y2="-4.06" layer="51"/>
<rectangle x1="-26.5" y1="0.9" x2="26.5" y2="1.4" layer="21"/>
<rectangle x1="-17.5" y1="-4.1" x2="17.5" y2="-2.5" layer="21"/>
</package>
<package name="M25H">
<description>&lt;b&gt;SUB-D&lt;/b&gt;</description>
<wire x1="19.304" y1="-17.526" x2="19.05" y2="-17.526" width="0.1524" layer="21"/>
<wire x1="19.304" y1="-17.526" x2="19.812" y2="-17.018" width="0.1524" layer="21" curve="90"/>
<wire x1="-19.812" y1="-17.018" x2="-19.304" y2="-17.526" width="0.1524" layer="21" curve="90"/>
<wire x1="19.431" y1="-11.684" x2="-19.431" y2="-11.684" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-7.62" x2="21.463" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-7.62" x2="26.543" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-7.62" x2="26.543" y2="-7.493" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-10.668" x2="26.543" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-11.176" x2="26.543" y2="-11.684" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-11.684" x2="-26.543" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-11.176" x2="-26.543" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-10.668" x2="-26.543" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-7.62" x2="-26.543" y2="-7.493" width="0.1524" layer="21"/>
<wire x1="21.463" y1="-10.668" x2="21.463" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="21.463" y1="-10.668" x2="21.336" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="20.701" y1="-7.62" x2="21.336" y2="-8.255" width="0.1524" layer="21" curve="-90"/>
<wire x1="21.463" y1="-7.62" x2="20.574" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="20.574" y1="-7.62" x2="20.32" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="21.336" y1="-8.255" x2="21.336" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="20.32" y1="-7.62" x2="20.32" y2="-6.858" width="0.1524" layer="21"/>
<wire x1="20.32" y1="-7.62" x2="-20.32" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="20.32" y1="-6.858" x2="-20.32" y2="-6.858" width="0.1524" layer="21"/>
<wire x1="-20.32" y1="-7.62" x2="-20.32" y2="-6.858" width="0.1524" layer="21"/>
<wire x1="-20.32" y1="-7.62" x2="-20.574" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-20.574" y1="-7.62" x2="-21.463" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="-10.668" x2="-21.463" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="-21.336" y1="-8.255" x2="-21.336" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="-21.336" y1="-10.668" x2="-21.463" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="-21.336" y1="-8.255" x2="-20.701" y2="-7.62" width="0.1524" layer="21" curve="-90"/>
<wire x1="25.908" y1="3.175" x2="26.543" y2="2.54" width="0.1524" layer="21"/>
<wire x1="26.543" y1="2.54" x2="26.543" y2="-7.493" width="0.1524" layer="21"/>
<wire x1="21.082" y1="3.175" x2="25.908" y2="3.175" width="0.1524" layer="21"/>
<wire x1="21.082" y1="3.175" x2="20.574" y2="2.667" width="0.1524" layer="21"/>
<wire x1="20.574" y1="2.667" x2="20.574" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-20.574" y1="2.667" x2="-20.574" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-20.574" y1="2.667" x2="-21.082" y2="3.175" width="0.1524" layer="21"/>
<wire x1="-21.082" y1="3.175" x2="-26.035" y2="3.175" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="3.175" x2="-26.543" y2="2.667" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="2.667" x2="-26.543" y2="-7.493" width="0.1524" layer="21"/>
<wire x1="-2.7686" y1="1.143" x2="-2.7686" y2="0.127" width="0.8128" layer="51"/>
<wire x1="0" y1="1.143" x2="0" y2="0.127" width="0.8128" layer="51"/>
<wire x1="2.7686" y1="1.143" x2="2.7686" y2="0.127" width="0.8128" layer="51"/>
<wire x1="1.3716" y1="-1.397" x2="1.3716" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="-1.3716" y1="-1.397" x2="-1.3716" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="-4.1402" y1="-1.397" x2="-4.1402" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="-5.5118" y1="1.143" x2="-5.5118" y2="0.127" width="0.8128" layer="51"/>
<wire x1="-6.9088" y1="-1.397" x2="-6.9088" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="-8.2804" y1="1.143" x2="-8.2804" y2="0.127" width="0.8128" layer="51"/>
<wire x1="-21.463" y1="-7.62" x2="-26.543" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-19.431" y1="-11.684" x2="-26.543" y2="-11.684" width="0.1524" layer="21"/>
<wire x1="-19.812" y1="-12.319" x2="-19.812" y2="-17.018" width="0.1524" layer="21"/>
<wire x1="-20.447" y1="-11.684" x2="-19.812" y2="-12.319" width="0.1524" layer="21" curve="-90"/>
<wire x1="26.543" y1="-11.684" x2="19.431" y2="-11.684" width="0.1524" layer="21"/>
<wire x1="19.812" y1="-12.319" x2="19.812" y2="-17.018" width="0.1524" layer="21"/>
<wire x1="19.812" y1="-12.319" x2="20.447" y2="-11.684" width="0.1524" layer="21" curve="-90"/>
<wire x1="-9.652" y1="-1.3716" x2="-9.652" y2="-2.3876" width="0.8128" layer="51"/>
<wire x1="-11.049" y1="1.143" x2="-11.049" y2="0.127" width="0.8128" layer="51"/>
<wire x1="4.1402" y1="-1.397" x2="4.1402" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="5.5118" y1="1.143" x2="5.5118" y2="0.127" width="0.8128" layer="51"/>
<wire x1="6.9088" y1="-1.397" x2="6.9088" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="8.2804" y1="1.143" x2="8.2804" y2="0.127" width="0.8128" layer="51"/>
<wire x1="9.652" y1="-1.397" x2="9.652" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="11.049" y1="1.143" x2="11.049" y2="0.127" width="0.8128" layer="51"/>
<wire x1="13.7922" y1="1.143" x2="13.7922" y2="0.127" width="0.8128" layer="51"/>
<wire x1="12.4206" y1="-1.397" x2="12.4206" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="16.5608" y1="1.143" x2="16.5608" y2="0.127" width="0.8128" layer="51"/>
<wire x1="15.1892" y1="-1.397" x2="15.1892" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="-13.7922" y1="1.143" x2="-13.7922" y2="0.127" width="0.8128" layer="51"/>
<wire x1="-12.4206" y1="-1.3716" x2="-12.4206" y2="-2.3876" width="0.8128" layer="51"/>
<wire x1="-15.1892" y1="-1.3716" x2="-15.1892" y2="-2.3876" width="0.8128" layer="51"/>
<wire x1="-16.5608" y1="1.143" x2="-16.5608" y2="0.127" width="0.8128" layer="51"/>
<wire x1="-19.05" y1="-17.526" x2="-19.304" y2="-17.526" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-17.526" x2="-19.05" y2="-17.526" width="0.1524" layer="21"/>
<circle x="23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<circle x="23.5204" y="0" radius="2.667" width="0" layer="42"/>
<circle x="23.5204" y="0" radius="2.667" width="0" layer="43"/>
<circle x="-23.5204" y="0" radius="2.667" width="0" layer="42"/>
<circle x="-23.5204" y="0" radius="2.667" width="0" layer="43"/>
<circle x="-23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<pad name="13" x="16.5608" y="1.27" drill="1.016" shape="octagon"/>
<pad name="12" x="13.7922" y="1.27" drill="1.016" shape="octagon"/>
<pad name="11" x="11.049" y="1.27" drill="1.016" shape="octagon"/>
<pad name="10" x="8.2804" y="1.27" drill="1.016" shape="octagon"/>
<pad name="9" x="5.5118" y="1.27" drill="1.016" shape="octagon"/>
<pad name="8" x="2.7686" y="1.27" drill="1.016" shape="octagon"/>
<pad name="7" x="0" y="1.27" drill="1.016" shape="octagon"/>
<pad name="6" x="-2.7686" y="1.27" drill="1.016" shape="octagon"/>
<pad name="5" x="-5.5118" y="1.27" drill="1.016" shape="octagon"/>
<pad name="4" x="-8.2804" y="1.27" drill="1.016" shape="octagon"/>
<pad name="3" x="-11.049" y="1.27" drill="1.016" shape="octagon"/>
<pad name="2" x="-13.7922" y="1.27" drill="1.016" shape="octagon"/>
<pad name="1" x="-16.5608" y="1.27" drill="1.016" shape="octagon"/>
<pad name="25" x="15.1892" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="24" x="12.4206" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="23" x="9.652" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="22" x="6.9088" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="21" x="4.1402" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="20" x="1.3716" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="19" x="-1.3716" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="18" x="-4.1402" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="17" x="-6.9088" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="16" x="-9.652" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="15" x="-12.4206" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="14" x="-15.1892" y="-1.27" drill="1.016" shape="octagon"/>
<text x="-26.416" y="4.445" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-17.78" y="-20.447" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-18.669" y="0.635" size="1.27" layer="21" ratio="10">1</text>
<text x="17.907" y="0.635" size="1.27" layer="21" ratio="10">13</text>
<text x="-19.812" y="-2.032" size="1.27" layer="21" ratio="10">14</text>
<text x="17.653" y="-1.905" size="1.27" layer="21" ratio="10">25</text>
<text x="-24.638" y="-6.858" size="1.27" layer="21" ratio="10" rot="R90">2,54</text>
<text x="-18.669" y="-9.779" size="1.27" layer="21" ratio="10">M25</text>
<rectangle x1="-26.543" y1="-11.684" x2="26.543" y2="-11.176" layer="21"/>
<rectangle x1="-20.32" y1="-7.62" x2="20.32" y2="-6.858" layer="21"/>
<rectangle x1="-16.9672" y1="-6.858" x2="-16.1544" y2="0.381" layer="21"/>
<rectangle x1="-15.5956" y1="-6.858" x2="-14.7828" y2="-2.159" layer="21"/>
<rectangle x1="-14.1986" y1="-6.858" x2="-13.3858" y2="0.381" layer="21"/>
<rectangle x1="-12.827" y1="-6.858" x2="-12.0142" y2="-2.159" layer="21"/>
<rectangle x1="-11.4554" y1="-6.858" x2="-10.6426" y2="0.381" layer="21"/>
<rectangle x1="-10.0584" y1="-6.858" x2="-9.2456" y2="-2.159" layer="21"/>
<rectangle x1="-8.6868" y1="-6.858" x2="-7.874" y2="0.381" layer="21"/>
<rectangle x1="-7.3152" y1="-6.858" x2="-6.5024" y2="-2.159" layer="21"/>
<rectangle x1="-5.9182" y1="-6.858" x2="-5.1054" y2="0.381" layer="21"/>
<rectangle x1="-4.5466" y1="-6.858" x2="-3.7338" y2="-2.159" layer="21"/>
<rectangle x1="-3.175" y1="-6.858" x2="-2.3622" y2="0.381" layer="21"/>
<rectangle x1="-1.778" y1="-6.858" x2="-0.9652" y2="-2.159" layer="21"/>
<rectangle x1="-0.4064" y1="-6.858" x2="0.4064" y2="0.381" layer="21"/>
<rectangle x1="0.9652" y1="-6.858" x2="1.778" y2="-2.159" layer="21"/>
<rectangle x1="2.3622" y1="-6.858" x2="3.175" y2="0.381" layer="21"/>
<rectangle x1="3.7338" y1="-6.858" x2="4.5466" y2="-2.159" layer="21"/>
<rectangle x1="5.1054" y1="-6.858" x2="5.9182" y2="0.381" layer="21"/>
<rectangle x1="6.5024" y1="-6.858" x2="7.3152" y2="-2.159" layer="21"/>
<rectangle x1="7.874" y1="-6.858" x2="8.6868" y2="0.381" layer="21"/>
<rectangle x1="9.2456" y1="-6.858" x2="10.0584" y2="-2.159" layer="21"/>
<rectangle x1="10.6426" y1="-6.858" x2="11.4554" y2="0.381" layer="21"/>
<rectangle x1="12.0142" y1="-6.858" x2="12.827" y2="-2.159" layer="21"/>
<rectangle x1="13.3858" y1="-6.858" x2="14.1986" y2="0.381" layer="21"/>
<rectangle x1="14.7828" y1="-6.858" x2="15.5956" y2="-2.159" layer="21"/>
<rectangle x1="16.1544" y1="-6.858" x2="16.9672" y2="0.381" layer="21"/>
<hole x="23.5204" y="0" drill="3.302"/>
<hole x="-23.5204" y="0" drill="3.302"/>
</package>
<package name="M25HP">
<description>&lt;b&gt;SUB-D&lt;/b&gt;</description>
<wire x1="19.304" y1="-17.526" x2="19.05" y2="-17.526" width="0.1524" layer="21"/>
<wire x1="19.304" y1="-17.526" x2="19.812" y2="-17.018" width="0.1524" layer="21" curve="90"/>
<wire x1="-19.812" y1="-17.018" x2="-19.304" y2="-17.526" width="0.1524" layer="21" curve="90"/>
<wire x1="19.431" y1="-11.684" x2="19.431" y2="-17.018" width="0.1524" layer="21"/>
<wire x1="19.431" y1="-11.684" x2="-19.431" y2="-11.684" width="0.1524" layer="21"/>
<wire x1="-19.431" y1="-11.684" x2="-19.431" y2="-17.018" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-7.62" x2="21.463" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-7.62" x2="26.543" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-7.62" x2="26.543" y2="-7.493" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-10.668" x2="24.003" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-10.668" x2="26.543" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-11.176" x2="26.543" y2="-11.684" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-11.684" x2="-26.543" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-11.176" x2="-26.543" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-10.668" x2="-26.543" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="-7.62" x2="-26.543" y2="-7.493" width="0.1524" layer="21"/>
<wire x1="24.003" y1="-10.414" x2="21.463" y2="-10.414" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-11.176" x2="24.003" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="24.003" y1="-11.176" x2="21.463" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="24.003" y1="-10.668" x2="24.003" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="24.003" y1="-10.668" x2="24.003" y2="-10.414" width="0.1524" layer="21"/>
<wire x1="21.463" y1="-10.414" x2="21.463" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="21.463" y1="-10.668" x2="21.463" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="26.543" y1="-7.493" x2="21.463" y2="-7.493" width="0.1524" layer="21"/>
<wire x1="21.463" y1="-7.493" x2="21.463" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="21.463" y1="-10.668" x2="21.336" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="21.336" y1="-10.668" x2="-21.336" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="20.701" y1="-7.62" x2="21.336" y2="-8.255" width="0.1524" layer="21" curve="-90"/>
<wire x1="21.463" y1="-7.62" x2="20.574" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="20.574" y1="-7.62" x2="20.32" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="21.336" y1="-8.255" x2="21.336" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="20.32" y1="-7.62" x2="20.32" y2="-6.858" width="0.1524" layer="21"/>
<wire x1="20.32" y1="-7.62" x2="-20.32" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="20.32" y1="-6.858" x2="-20.32" y2="-6.858" width="0.1524" layer="21"/>
<wire x1="-20.32" y1="-7.62" x2="-20.32" y2="-6.858" width="0.1524" layer="21"/>
<wire x1="-20.32" y1="-7.62" x2="-20.574" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-20.574" y1="-7.62" x2="-21.463" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="-7.493" x2="-26.543" y2="-7.493" width="0.1524" layer="21"/>
<wire x1="-24.003" y1="-10.668" x2="-24.003" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="-24.003" y1="-10.668" x2="-26.543" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="-24.003" y1="-11.176" x2="-26.543" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="-10.414" x2="-24.003" y2="-10.414" width="0.1524" layer="21"/>
<wire x1="-24.003" y1="-10.668" x2="-24.003" y2="-10.414" width="0.1524" layer="21"/>
<wire x1="21.463" y1="-11.176" x2="-21.463" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="-11.176" x2="-24.003" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="-10.414" x2="-21.463" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="-10.668" x2="-21.463" y2="-11.176" width="0.1524" layer="21"/>
<wire x1="-21.336" y1="-8.255" x2="-21.336" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="-21.336" y1="-10.668" x2="-21.463" y2="-10.668" width="0.1524" layer="21"/>
<wire x1="-21.336" y1="-8.255" x2="-20.701" y2="-7.62" width="0.1524" layer="21" curve="-90"/>
<wire x1="25.908" y1="3.175" x2="26.543" y2="2.54" width="0.1524" layer="21"/>
<wire x1="26.543" y1="2.54" x2="26.543" y2="-7.493" width="0.1524" layer="21"/>
<wire x1="21.082" y1="3.175" x2="25.908" y2="3.175" width="0.1524" layer="21"/>
<wire x1="21.082" y1="3.175" x2="20.574" y2="2.667" width="0.1524" layer="21"/>
<wire x1="20.574" y1="2.667" x2="20.574" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-20.574" y1="2.667" x2="-20.574" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-20.574" y1="2.667" x2="-21.082" y2="3.175" width="0.1524" layer="21"/>
<wire x1="-21.082" y1="3.175" x2="-26.035" y2="3.175" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="3.175" x2="-26.543" y2="2.667" width="0.1524" layer="21"/>
<wire x1="-26.543" y1="2.667" x2="-26.543" y2="-7.493" width="0.1524" layer="21"/>
<wire x1="-2.7686" y1="1.143" x2="-2.7686" y2="0.127" width="0.8128" layer="51"/>
<wire x1="0" y1="1.143" x2="0" y2="0.127" width="0.8128" layer="51"/>
<wire x1="2.7686" y1="1.143" x2="2.7686" y2="0.127" width="0.8128" layer="51"/>
<wire x1="1.3716" y1="-1.397" x2="1.3716" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="-1.3716" y1="-1.397" x2="-1.3716" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="-4.1402" y1="-1.397" x2="-4.1402" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="-5.5118" y1="1.143" x2="-5.5118" y2="0.127" width="0.8128" layer="51"/>
<wire x1="-6.9088" y1="-1.397" x2="-6.9088" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="-8.2804" y1="1.143" x2="-8.2804" y2="0.127" width="0.8128" layer="51"/>
<wire x1="-5.08" y1="-13.97" x2="-2.794" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="-13.97" x2="-2.794" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-2.794" y1="-16.256" x2="-3.175" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-16.256" x2="-3.175" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-14.351" x2="-4.699" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-4.699" y1="-14.351" x2="-4.699" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-4.699" y1="-16.256" x2="-5.08" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-16.256" x2="-5.08" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-1.143" y1="-13.97" x2="1.143" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="1.143" y1="-13.97" x2="1.143" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="1.143" y1="-16.256" x2="0.762" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="0.762" y1="-16.256" x2="0.762" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="0.762" y1="-14.351" x2="-0.762" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-0.762" y1="-14.351" x2="-0.762" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-0.762" y1="-16.256" x2="-1.143" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-1.143" y1="-16.256" x2="-1.143" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="2.794" y1="-13.97" x2="5.08" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-13.97" x2="5.08" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-16.256" x2="4.699" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="4.699" y1="-16.256" x2="4.699" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="4.699" y1="-14.351" x2="3.175" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-14.351" x2="3.175" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-16.256" x2="2.794" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="2.794" y1="-16.256" x2="2.794" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="-7.493" x2="-21.463" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-21.463" y1="-7.62" x2="-26.543" y2="-7.62" width="0.1524" layer="21"/>
<wire x1="-19.431" y1="-11.684" x2="-26.543" y2="-11.684" width="0.1524" layer="21"/>
<wire x1="-19.812" y1="-12.319" x2="-19.812" y2="-17.018" width="0.1524" layer="21"/>
<wire x1="-20.447" y1="-11.684" x2="-19.812" y2="-12.319" width="0.1524" layer="21" curve="-90"/>
<wire x1="26.543" y1="-11.684" x2="19.431" y2="-11.684" width="0.1524" layer="21"/>
<wire x1="19.812" y1="-12.319" x2="19.812" y2="-17.018" width="0.1524" layer="21"/>
<wire x1="19.812" y1="-12.319" x2="20.447" y2="-11.684" width="0.1524" layer="21" curve="-90"/>
<wire x1="-9.652" y1="-1.3716" x2="-9.652" y2="-2.3876" width="0.8128" layer="51"/>
<wire x1="-11.049" y1="1.143" x2="-11.049" y2="0.127" width="0.8128" layer="51"/>
<wire x1="4.1402" y1="-1.397" x2="4.1402" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="5.5118" y1="1.143" x2="5.5118" y2="0.127" width="0.8128" layer="51"/>
<wire x1="6.9088" y1="-1.397" x2="6.9088" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="8.2804" y1="1.143" x2="8.2804" y2="0.127" width="0.8128" layer="51"/>
<wire x1="-9.017" y1="-13.97" x2="-6.731" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-6.731" y1="-13.97" x2="-6.731" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-6.731" y1="-16.256" x2="-7.112" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-7.112" y1="-16.256" x2="-7.112" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-7.112" y1="-14.351" x2="-8.636" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-8.636" y1="-14.351" x2="-8.636" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-8.636" y1="-16.256" x2="-9.017" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-9.017" y1="-16.256" x2="-9.017" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="6.731" y1="-13.97" x2="9.017" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="9.017" y1="-13.97" x2="9.017" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="9.017" y1="-16.256" x2="8.636" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="8.636" y1="-16.256" x2="8.636" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="8.636" y1="-14.351" x2="7.112" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="7.112" y1="-14.351" x2="7.112" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="7.112" y1="-16.256" x2="6.731" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="6.731" y1="-16.256" x2="6.731" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="9.652" y1="-1.397" x2="9.652" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="11.049" y1="1.143" x2="11.049" y2="0.127" width="0.8128" layer="51"/>
<wire x1="13.7922" y1="1.143" x2="13.7922" y2="0.127" width="0.8128" layer="51"/>
<wire x1="12.4206" y1="-1.397" x2="12.4206" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="16.5608" y1="1.143" x2="16.5608" y2="0.127" width="0.8128" layer="51"/>
<wire x1="15.1892" y1="-1.397" x2="15.1892" y2="-2.413" width="0.8128" layer="51"/>
<wire x1="-13.7922" y1="1.143" x2="-13.7922" y2="0.127" width="0.8128" layer="51"/>
<wire x1="-12.4206" y1="-1.3716" x2="-12.4206" y2="-2.3876" width="0.8128" layer="51"/>
<wire x1="-15.1892" y1="-1.3716" x2="-15.1892" y2="-2.3876" width="0.8128" layer="51"/>
<wire x1="-16.5608" y1="1.143" x2="-16.5608" y2="0.127" width="0.8128" layer="51"/>
<wire x1="-12.954" y1="-13.97" x2="-10.668" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-10.668" y1="-13.97" x2="-10.668" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-10.668" y1="-16.256" x2="-11.049" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-11.049" y1="-16.256" x2="-11.049" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-11.049" y1="-14.351" x2="-12.573" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-12.573" y1="-14.351" x2="-12.573" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-12.573" y1="-16.256" x2="-12.954" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-12.954" y1="-16.256" x2="-12.954" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="10.668" y1="-13.97" x2="12.954" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="12.954" y1="-13.97" x2="12.954" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="12.954" y1="-16.256" x2="12.573" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="12.573" y1="-16.256" x2="12.573" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="12.573" y1="-14.351" x2="11.049" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="11.049" y1="-14.351" x2="11.049" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="11.049" y1="-16.256" x2="10.668" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="10.668" y1="-16.256" x2="10.668" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-19.05" y1="-12.192" x2="-19.05" y2="-17.526" width="0.1524" layer="21"/>
<wire x1="-19.05" y1="-17.526" x2="-19.304" y2="-17.526" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-12.192" x2="19.05" y2="-17.526" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-17.526" x2="-19.05" y2="-17.526" width="0.1524" layer="21"/>
<circle x="23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<circle x="-23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<pad name="13" x="16.5608" y="1.27" drill="1.016" shape="octagon"/>
<pad name="12" x="13.7922" y="1.27" drill="1.016" shape="octagon"/>
<pad name="11" x="11.049" y="1.27" drill="1.016" shape="octagon"/>
<pad name="10" x="8.2804" y="1.27" drill="1.016" shape="octagon"/>
<pad name="9" x="5.5118" y="1.27" drill="1.016" shape="octagon"/>
<pad name="8" x="2.7686" y="1.27" drill="1.016" shape="octagon"/>
<pad name="7" x="0" y="1.27" drill="1.016" shape="octagon"/>
<pad name="6" x="-2.7686" y="1.27" drill="1.016" shape="octagon"/>
<pad name="5" x="-5.5118" y="1.27" drill="1.016" shape="octagon"/>
<pad name="4" x="-8.2804" y="1.27" drill="1.016" shape="octagon"/>
<pad name="3" x="-11.049" y="1.27" drill="1.016" shape="octagon"/>
<pad name="2" x="-13.7922" y="1.27" drill="1.016" shape="octagon"/>
<pad name="1" x="-16.5608" y="1.27" drill="1.016" shape="octagon"/>
<pad name="25" x="15.1892" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="24" x="12.4206" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="23" x="9.652" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="22" x="6.9088" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="21" x="4.1402" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="20" x="1.3716" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="19" x="-1.3716" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="18" x="-4.1402" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="17" x="-6.9088" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="16" x="-9.652" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="15" x="-12.4206" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="14" x="-15.1892" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="G1" x="-23.5204" y="0" drill="3.302" diameter="5.08"/>
<pad name="G2" x="23.5204" y="0" drill="3.302" diameter="5.08"/>
<text x="-26.416" y="4.445" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-17.78" y="-20.447" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-18.669" y="0.635" size="1.27" layer="21" ratio="10">1</text>
<text x="17.907" y="0.635" size="1.27" layer="21" ratio="10">13</text>
<text x="-19.812" y="-2.032" size="1.27" layer="21" ratio="10">14</text>
<text x="17.653" y="-1.905" size="1.27" layer="21" ratio="10">25</text>
<text x="-24.638" y="-6.858" size="1.27" layer="21" ratio="10" rot="R90">2,54</text>
<text x="-18.669" y="-9.779" size="1.27" layer="21" ratio="10">M25</text>
<rectangle x1="20.701" y1="-10.668" x2="20.955" y2="-8.255" layer="21"/>
<rectangle x1="-20.955" y1="-10.668" x2="-20.701" y2="-8.255" layer="21"/>
<rectangle x1="23.0632" y1="-7.62" x2="24.0792" y2="-5.969" layer="21"/>
<rectangle x1="-24.0792" y1="-7.62" x2="-23.0632" y2="-5.969" layer="21"/>
<rectangle x1="-26.543" y1="-11.684" x2="26.543" y2="-11.176" layer="21"/>
<rectangle x1="-20.32" y1="-7.62" x2="20.32" y2="-6.858" layer="21"/>
<rectangle x1="-16.9672" y1="-6.858" x2="-16.1544" y2="0.381" layer="21"/>
<rectangle x1="-15.5956" y1="-6.858" x2="-14.7828" y2="-2.159" layer="21"/>
<rectangle x1="-14.1986" y1="-6.858" x2="-13.3858" y2="0.381" layer="21"/>
<rectangle x1="-12.827" y1="-6.858" x2="-12.0142" y2="-2.159" layer="21"/>
<rectangle x1="-11.4554" y1="-6.858" x2="-10.6426" y2="0.381" layer="21"/>
<rectangle x1="-10.0584" y1="-6.858" x2="-9.2456" y2="-2.159" layer="21"/>
<rectangle x1="-8.6868" y1="-6.858" x2="-7.874" y2="0.381" layer="21"/>
<rectangle x1="-7.3152" y1="-6.858" x2="-6.5024" y2="-2.159" layer="21"/>
<rectangle x1="-5.9182" y1="-6.858" x2="-5.1054" y2="0.381" layer="21"/>
<rectangle x1="-4.5466" y1="-6.858" x2="-3.7338" y2="-2.159" layer="21"/>
<rectangle x1="-3.175" y1="-6.858" x2="-2.3622" y2="0.381" layer="21"/>
<rectangle x1="-1.778" y1="-6.858" x2="-0.9652" y2="-2.159" layer="21"/>
<rectangle x1="-0.4064" y1="-6.858" x2="0.4064" y2="0.381" layer="21"/>
<rectangle x1="0.9652" y1="-6.858" x2="1.778" y2="-2.159" layer="21"/>
<rectangle x1="2.3622" y1="-6.858" x2="3.175" y2="0.381" layer="21"/>
<rectangle x1="3.7338" y1="-6.858" x2="4.5466" y2="-2.159" layer="21"/>
<rectangle x1="5.1054" y1="-6.858" x2="5.9182" y2="0.381" layer="21"/>
<rectangle x1="6.5024" y1="-6.858" x2="7.3152" y2="-2.159" layer="21"/>
<rectangle x1="7.874" y1="-6.858" x2="8.6868" y2="0.381" layer="21"/>
<rectangle x1="9.2456" y1="-6.858" x2="10.0584" y2="-2.159" layer="21"/>
<rectangle x1="10.6426" y1="-6.858" x2="11.4554" y2="0.381" layer="21"/>
<rectangle x1="12.0142" y1="-6.858" x2="12.827" y2="-2.159" layer="21"/>
<rectangle x1="13.3858" y1="-6.858" x2="14.1986" y2="0.381" layer="21"/>
<rectangle x1="14.7828" y1="-6.858" x2="15.5956" y2="-2.159" layer="21"/>
<rectangle x1="16.1544" y1="-6.858" x2="16.9672" y2="0.381" layer="21"/>
</package>
<package name="M25V">
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
<wire x1="-18.034" y1="4.318" x2="18.034" y2="4.318" width="0.1524" layer="21"/>
<wire x1="19.7358" y1="2.3114" x2="18.9738" y2="-2.8702" width="0.1524" layer="21"/>
<wire x1="18.034" y1="4.318" x2="19.728" y2="2.2521" width="0.1524" layer="21" curve="-101.297755"/>
<wire x1="-18.9484" y1="-2.9464" x2="-19.7358" y2="2.3368" width="0.1524" layer="21"/>
<wire x1="-19.7277" y1="2.2521" x2="-18.034" y2="4.318" width="0.1524" layer="21" curve="-101.307706"/>
<wire x1="-17.272" y1="-4.318" x2="17.272" y2="-4.318" width="0.1524" layer="21"/>
<wire x1="-18.984" y1="-2.8191" x2="-17.272" y2="-4.318" width="0.1524" layer="21" curve="82.402958"/>
<wire x1="17.272" y1="-4.318" x2="18.9963" y2="-2.6849" width="0.1524" layer="21" curve="86.865803"/>
<circle x="23.5204" y="0" radius="2.667" width="0" layer="42"/>
<circle x="23.5204" y="0" radius="2.667" width="0" layer="43"/>
<circle x="-23.5204" y="0" radius="2.667" width="0" layer="42"/>
<circle x="-23.5204" y="0" radius="2.667" width="0" layer="43"/>
<circle x="-23.5204" y="-0.0254" radius="1.651" width="0.1524" layer="21"/>
<circle x="23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<circle x="-2.7686" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-4.1402" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-5.5118" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="0" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-1.3716" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="1.3716" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="2.7686" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="5.5118" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="4.1402" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="6.9088" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="8.2804" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="9.652" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="11.049" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="12.4206" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="13.7922" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="16.5608" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="15.1892" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-8.2804" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-6.9088" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-9.652" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-11.049" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-13.7922" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-16.5608" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-12.4206" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-15.1892" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<pad name="13" x="16.5608" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="12" x="13.7922" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="11" x="11.049" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="10" x="8.2804" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="9" x="5.5118" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="8" x="2.7686" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="7" x="0" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="6" x="-2.7686" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="5" x="-5.5118" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="4" x="-8.2804" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="3" x="-11.049" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="2" x="-13.7922" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="1" x="-16.5608" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="25" x="15.1892" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="24" x="12.4206" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="23" x="9.652" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="22" x="6.9088" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="21" x="4.1402" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="20" x="1.3716" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="19" x="-1.3716" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="18" x="-4.1402" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="17" x="-6.9088" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="16" x="-9.652" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="15" x="-12.4206" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="14" x="-15.1892" y="-1.4224" drill="1.016" shape="octagon"/>
<text x="-26.289" y="6.985" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="6.985" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-16.764" y="2.413" size="0.9906" layer="21" ratio="12">1</text>
<text x="-14.097" y="2.413" size="0.9906" layer="21" ratio="12">2</text>
<text x="-11.43" y="2.413" size="0.9906" layer="21" ratio="12">3</text>
<text x="-8.763" y="2.413" size="0.9906" layer="21" ratio="12">4</text>
<text x="-5.969" y="2.413" size="0.9906" layer="21" ratio="12">5</text>
<text x="-3.175" y="2.413" size="0.9906" layer="21" ratio="12">6</text>
<text x="-0.381" y="2.413" size="0.9906" layer="21" ratio="12">7</text>
<text x="2.413" y="2.413" size="0.9906" layer="21" ratio="12">8</text>
<text x="5.08" y="2.413" size="0.9906" layer="21" ratio="12">9</text>
<text x="7.62" y="2.413" size="0.9906" layer="21" ratio="12">10</text>
<text x="10.287" y="2.413" size="0.9906" layer="21" ratio="12">11</text>
<text x="12.954" y="2.413" size="0.9906" layer="21" ratio="12">12</text>
<text x="15.875" y="2.413" size="0.9906" layer="21" ratio="12">13</text>
<text x="-16.002" y="-3.429" size="0.9906" layer="21" ratio="12">14</text>
<text x="-13.208" y="-3.429" size="0.9906" layer="21" ratio="12">15</text>
<text x="-10.414" y="-3.429" size="0.9906" layer="21" ratio="12">16</text>
<text x="-7.747" y="-3.429" size="0.9906" layer="21" ratio="12">17</text>
<text x="-5.08" y="-3.429" size="0.9906" layer="21" ratio="12">18</text>
<text x="-2.159" y="-3.429" size="0.9906" layer="21" ratio="12">19</text>
<text x="0.508" y="-3.429" size="0.9906" layer="21" ratio="12">20</text>
<text x="3.302" y="-3.429" size="0.9906" layer="21" ratio="12">21</text>
<text x="6.096" y="-3.429" size="0.9906" layer="21" ratio="12">22</text>
<text x="8.89" y="-3.429" size="0.9906" layer="21" ratio="12">23</text>
<text x="11.557" y="-3.429" size="0.9906" layer="21" ratio="12">24</text>
<text x="14.351" y="-3.429" size="0.9906" layer="21" ratio="12">25</text>
<hole x="23.5204" y="0" drill="3.302"/>
<hole x="-23.5204" y="0" drill="3.302"/>
</package>
<package name="M25VP">
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
<wire x1="-18.034" y1="4.318" x2="18.034" y2="4.318" width="0.1524" layer="21"/>
<wire x1="19.7358" y1="2.3114" x2="18.9738" y2="-2.8702" width="0.1524" layer="21"/>
<wire x1="18.034" y1="4.318" x2="19.728" y2="2.2521" width="0.1524" layer="21" curve="-101.297755"/>
<wire x1="-18.9484" y1="-2.9464" x2="-19.7358" y2="2.3368" width="0.1524" layer="21"/>
<wire x1="-19.7277" y1="2.2521" x2="-18.034" y2="4.318" width="0.1524" layer="21" curve="-101.307706"/>
<wire x1="-17.272" y1="-4.318" x2="17.272" y2="-4.318" width="0.1524" layer="21"/>
<wire x1="-18.984" y1="-2.8191" x2="-17.272" y2="-4.318" width="0.1524" layer="21" curve="82.402958"/>
<wire x1="17.272" y1="-4.318" x2="18.9963" y2="-2.6849" width="0.1524" layer="21" curve="86.865803"/>
<circle x="-23.5204" y="-0.0254" radius="1.651" width="0.1524" layer="21"/>
<circle x="23.5204" y="0" radius="1.651" width="0.1524" layer="21"/>
<circle x="-2.7686" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-4.1402" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-5.5118" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="0" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-1.3716" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="1.3716" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="2.7686" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="5.5118" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="4.1402" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="6.9088" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="8.2804" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="9.652" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="11.049" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="12.4206" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="13.7922" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="16.5608" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="15.1892" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-8.2804" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-6.9088" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-9.652" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-11.049" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-13.7922" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-16.5608" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-12.4206" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-15.1892" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<pad name="13" x="16.5608" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="12" x="13.7922" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="11" x="11.049" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="10" x="8.2804" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="9" x="5.5118" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="8" x="2.7686" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="7" x="0" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="6" x="-2.7686" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="5" x="-5.5118" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="4" x="-8.2804" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="3" x="-11.049" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="2" x="-13.7922" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="1" x="-16.5608" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="25" x="15.1892" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="24" x="12.4206" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="23" x="9.652" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="22" x="6.9088" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="21" x="4.1402" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="20" x="1.3716" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="19" x="-1.3716" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="18" x="-4.1402" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="17" x="-6.9088" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="16" x="-9.652" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="15" x="-12.4206" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="14" x="-15.1892" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="G1" x="-23.5204" y="0" drill="3.302" diameter="5.08"/>
<pad name="G2" x="23.5204" y="0" drill="3.302" diameter="5.08"/>
<text x="-26.289" y="6.985" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="6.985" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-16.764" y="2.413" size="0.9906" layer="21" ratio="12">1</text>
<text x="-14.097" y="2.413" size="0.9906" layer="21" ratio="12">2</text>
<text x="-11.43" y="2.413" size="0.9906" layer="21" ratio="12">3</text>
<text x="-8.763" y="2.413" size="0.9906" layer="21" ratio="12">4</text>
<text x="-5.969" y="2.413" size="0.9906" layer="21" ratio="12">5</text>
<text x="-3.175" y="2.413" size="0.9906" layer="21" ratio="12">6</text>
<text x="-0.381" y="2.413" size="0.9906" layer="21" ratio="12">7</text>
<text x="2.413" y="2.413" size="0.9906" layer="21" ratio="12">8</text>
<text x="5.08" y="2.413" size="0.9906" layer="21" ratio="12">9</text>
<text x="7.62" y="2.413" size="0.9906" layer="21" ratio="12">10</text>
<text x="10.287" y="2.413" size="0.9906" layer="21" ratio="12">11</text>
<text x="12.954" y="2.413" size="0.9906" layer="21" ratio="12">12</text>
<text x="15.875" y="2.413" size="0.9906" layer="21" ratio="12">13</text>
<text x="-16.002" y="-3.429" size="0.9906" layer="21" ratio="12">14</text>
<text x="-13.208" y="-3.429" size="0.9906" layer="21" ratio="12">15</text>
<text x="-10.414" y="-3.429" size="0.9906" layer="21" ratio="12">16</text>
<text x="-7.747" y="-3.429" size="0.9906" layer="21" ratio="12">17</text>
<text x="-5.08" y="-3.429" size="0.9906" layer="21" ratio="12">18</text>
<text x="-2.159" y="-3.429" size="0.9906" layer="21" ratio="12">19</text>
<text x="0.508" y="-3.429" size="0.9906" layer="21" ratio="12">20</text>
<text x="3.302" y="-3.429" size="0.9906" layer="21" ratio="12">21</text>
<text x="6.096" y="-3.429" size="0.9906" layer="21" ratio="12">22</text>
<text x="8.89" y="-3.429" size="0.9906" layer="21" ratio="12">23</text>
<text x="11.557" y="-3.429" size="0.9906" layer="21" ratio="12">24</text>
<text x="14.351" y="-3.429" size="0.9906" layer="21" ratio="12">25</text>
</package>
<package name="M25VB">
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
<wire x1="18.034" y1="-4.318" x2="-18.034" y2="-4.318" width="0.1524" layer="21"/>
<wire x1="-19.7358" y1="-2.3114" x2="-18.9738" y2="2.8702" width="0.1524" layer="21"/>
<wire x1="-19.728" y1="-2.2521" x2="-18.034" y2="-4.318" width="0.1524" layer="21" curve="101.297755"/>
<wire x1="18.9484" y1="2.9464" x2="19.7358" y2="-2.3368" width="0.1524" layer="21"/>
<wire x1="18.034" y1="-4.318" x2="19.7277" y2="-2.2521" width="0.1524" layer="21" curve="101.307706"/>
<wire x1="17.272" y1="4.318" x2="-17.272" y2="4.318" width="0.1524" layer="21"/>
<wire x1="17.272" y1="4.318" x2="18.984" y2="2.8191" width="0.1524" layer="21" curve="-82.402958"/>
<wire x1="-18.9963" y1="2.6849" x2="-17.272" y2="4.318" width="0.1524" layer="21" curve="-86.865803"/>
<circle x="2.7686" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="4.1402" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="5.5118" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="0" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="1.3716" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-1.3716" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-2.7686" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-5.5118" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-4.1402" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-6.9088" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-8.2804" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-9.652" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-11.049" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-12.4206" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-13.7922" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-16.5608" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="-15.1892" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="8.2804" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="6.9088" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="9.652" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="11.049" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="13.7922" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="16.5608" y="-1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="12.4206" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<circle x="15.1892" y="1.4224" radius="0.254" width="0.4064" layer="51"/>
<pad name="13" x="-16.5608" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="12" x="-13.7922" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="11" x="-11.049" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="10" x="-8.2804" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="9" x="-5.5118" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="8" x="-2.7686" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="7" x="0" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="6" x="2.7686" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="5" x="5.5118" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="4" x="8.2804" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="3" x="11.049" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="2" x="13.7922" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="1" x="16.5608" y="-1.4224" drill="1.016" shape="octagon"/>
<pad name="25" x="-15.1892" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="24" x="-12.4206" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="23" x="-9.652" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="22" x="-6.9088" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="21" x="-4.1402" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="20" x="-1.3716" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="19" x="1.3716" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="18" x="4.1402" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="17" x="6.9088" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="16" x="9.652" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="15" x="12.4206" y="1.4224" drill="1.016" shape="octagon"/>
<pad name="14" x="15.1892" y="1.4224" drill="1.016" shape="octagon"/>
<text x="-26.289" y="6.985" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="6.985" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="16.764" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">1</text>
<text x="14.097" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">2</text>
<text x="11.43" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">3</text>
<text x="8.763" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">4</text>
<text x="5.969" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">5</text>
<text x="3.175" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">6</text>
<text x="0.381" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">7</text>
<text x="-2.413" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">8</text>
<text x="-5.08" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">9</text>
<text x="-7.62" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">10</text>
<text x="-10.287" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">11</text>
<text x="-12.954" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">12</text>
<text x="-15.875" y="-2.413" size="0.9906" layer="21" ratio="12" rot="R180">13</text>
<text x="16.002" y="3.429" size="0.9906" layer="21" ratio="12" rot="R180">14</text>
<text x="13.208" y="3.429" size="0.9906" layer="21" ratio="12" rot="R180">15</text>
<text x="10.414" y="3.429" size="0.9906" layer="21" ratio="12" rot="R180">16</text>
<text x="7.747" y="3.429" size="0.9906" layer="21" ratio="12" rot="R180">17</text>
<text x="5.08" y="3.429" size="0.9906" layer="21" ratio="12" rot="R180">18</text>
<text x="2.159" y="3.429" size="0.9906" layer="21" ratio="12" rot="R180">19</text>
<text x="-0.508" y="3.429" size="0.9906" layer="21" ratio="12" rot="R180">20</text>
<text x="-3.302" y="3.429" size="0.9906" layer="21" ratio="12" rot="R180">21</text>
<text x="-6.096" y="3.429" size="0.9906" layer="21" ratio="12" rot="R180">22</text>
<text x="-8.89" y="3.429" size="0.9906" layer="21" ratio="12" rot="R180">23</text>
<text x="-11.557" y="3.429" size="0.9906" layer="21" ratio="12" rot="R180">24</text>
<text x="-14.351" y="3.429" size="0.9906" layer="21" ratio="12" rot="R180">25</text>
</package>
<package name="182-025-MALE">
<description>&lt;b&gt;NORCOMP Right Angle D-Sub Connector&lt;/b&gt;, 182 Economy Series (.318" Footprint)&lt;p&gt;
Source: http://www.norcomp.net/ .. 182-yyy-113Ryy1Rev3.pdf</description>
<wire x1="19.3294" y1="-5.751" x2="19.8374" y2="-5.243" width="0.1524" layer="21" curve="90"/>
<wire x1="-19.8374" y1="-5.243" x2="-19.3294" y2="-5.751" width="0.1524" layer="21" curve="90"/>
<wire x1="26.5684" y1="12.875" x2="26.5684" y2="2.885" width="0.1524" layer="21"/>
<wire x1="26.5684" y1="2.885" x2="26.5684" y2="0.091" width="0.1524" layer="21"/>
<wire x1="-26.5684" y1="12.875" x2="-26.5684" y2="0.091" width="0.1524" layer="21"/>
<wire x1="26.5684" y1="0.091" x2="-26.5684" y2="0.091" width="0.1524" layer="21"/>
<wire x1="-19.8374" y1="-0.544" x2="-19.8374" y2="-5.243" width="0.1524" layer="21"/>
<wire x1="-20.4724" y1="0.091" x2="-19.8374" y2="-0.544" width="0.1524" layer="21" curve="-90"/>
<wire x1="19.8374" y1="-0.544" x2="19.8374" y2="-5.243" width="0.1524" layer="21"/>
<wire x1="19.8374" y1="-0.544" x2="20.4724" y2="0.091" width="0.1524" layer="21" curve="-90"/>
<wire x1="19.3294" y1="-5.751" x2="-19.3294" y2="-5.751" width="0.1524" layer="21"/>
<wire x1="26.5684" y1="2.885" x2="-26.5684" y2="2.885" width="0.1524" layer="21"/>
<wire x1="-20.5994" y1="12.875" x2="-20.5994" y2="3.082" width="0.1524" layer="21"/>
<wire x1="20.5994" y1="3.082" x2="20.5994" y2="12.875" width="0.1524" layer="21"/>
<wire x1="26.5684" y1="12.8926" x2="-26.5684" y2="12.8926" width="0.1524" layer="21"/>
<pad name="5" x="-5.5372" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="4" x="-8.3058" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="3" x="-11.0744" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="2" x="-13.843" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="1" x="-16.6116" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="14" x="-15.2273" y="8.0797" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="6" x="-2.7686" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="7" x="0" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="8" x="2.7686" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="9" x="5.5372" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="10" x="8.3058" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="11" x="11.0744" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="12" x="13.843" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="13" x="16.6116" y="10.9233" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="15" x="-12.4587" y="8.0797" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="16" x="-9.6901" y="8.0797" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="17" x="-6.9215" y="8.0797" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="18" x="-4.1529" y="8.0797" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="19" x="-1.3843" y="8.0797" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="20" x="1.3843" y="8.0797" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="21" x="4.1529" y="8.0797" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="22" x="6.9215" y="8.0797" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="23" x="9.6901" y="8.0797" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="24" x="12.4587" y="8.0797" drill="1.1938" diameter="1.524" shape="octagon"/>
<pad name="25" x="15.2273" y="8.0797" drill="1.1938" diameter="1.524" shape="octagon"/>
<text x="-20.32" y="13.335" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-8.89" y="13.335" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-18.6944" y="9.9782" size="1.27" layer="21" ratio="10">1</text>
<text x="18.3134" y="9.9782" size="1.27" layer="21" ratio="10">13</text>
<text x="18.3134" y="7.4382" size="1.27" layer="21" ratio="10">25</text>
<text x="-18.6944" y="7.3112" size="1.27" layer="21" ratio="10">14</text>
<hole x="-23.5204" y="9.5009" drill="3.048"/>
<hole x="23.5204" y="9.5009" drill="3.048"/>
</package>
<package name="M25SMT">
<description>&lt;b&gt;D-Sub Steckverbinder&lt;/b&gt; Abgewinkelte Messerleisten SMT, Einbauhöhe 3,6mm&lt;p&gt;
Source: ERNI-D-Sub-Conmnectors-d.pdf / www.erni.com</description>
<wire x1="19.279" y1="-17.526" x2="19.025" y2="-17.526" width="0.1524" layer="21"/>
<wire x1="19.279" y1="-17.526" x2="19.787" y2="-17.018" width="0.1524" layer="21" curve="90"/>
<wire x1="-19.787" y1="-17.018" x2="-19.279" y2="-17.526" width="0.1524" layer="21" curve="90"/>
<wire x1="19.406" y1="-11.684" x2="19.406" y2="-17.018" width="0.1524" layer="21"/>
<wire x1="19.406" y1="-11.684" x2="-19.406" y2="-11.684" width="0.1524" layer="21"/>
<wire x1="-19.406" y1="-11.684" x2="-19.406" y2="-17.018" width="0.1524" layer="21"/>
<wire x1="26.518" y1="-8.92" x2="20.295" y2="-8.92" width="0.1524" layer="21"/>
<wire x1="26.518" y1="-8.92" x2="26.518" y2="-11.684" width="0.1524" layer="21"/>
<wire x1="26.518" y1="-8.92" x2="26.47" y2="-8.793" width="0.1524" layer="21"/>
<wire x1="-26.518" y1="-11.684" x2="-26.518" y2="-8.92" width="0.1524" layer="21"/>
<wire x1="-26.518" y1="-8.92" x2="-26.47" y2="-8.793" width="0.1524" layer="21"/>
<wire x1="20.295" y1="-8.92" x2="20.295" y2="-8.158" width="0.1524" layer="21"/>
<wire x1="20.295" y1="-8.92" x2="-20.295" y2="-8.92" width="0.1524" layer="21"/>
<wire x1="20.295" y1="-8.158" x2="-20.295" y2="-8.158" width="0.1524" layer="21"/>
<wire x1="-20.295" y1="-8.92" x2="-20.295" y2="-8.158" width="0.1524" layer="21"/>
<wire x1="26.47" y1="3.016" x2="26.47" y2="-8.793" width="0.1524" layer="21"/>
<wire x1="-26.47" y1="3.016" x2="-26.47" y2="-8.793" width="0.1524" layer="21"/>
<wire x1="2.769" y1="-13.97" x2="5.055" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="5.055" y1="-13.97" x2="5.055" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="5.055" y1="-16.256" x2="4.674" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="4.674" y1="-16.256" x2="4.674" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="4.674" y1="-14.351" x2="3.15" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="3.15" y1="-14.351" x2="3.15" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="3.15" y1="-16.256" x2="2.769" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="2.769" y1="-16.256" x2="2.769" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-20.295" y1="-8.92" x2="-26.518" y2="-8.92" width="0.1524" layer="21"/>
<wire x1="-19.406" y1="-11.684" x2="-26.518" y2="-11.684" width="0.1524" layer="21"/>
<wire x1="-19.787" y1="-12.319" x2="-19.787" y2="-17.018" width="0.1524" layer="21"/>
<wire x1="-20.422" y1="-11.684" x2="-19.787" y2="-12.319" width="0.1524" layer="21" curve="-90"/>
<wire x1="26.518" y1="-11.684" x2="19.406" y2="-11.684" width="0.1524" layer="21"/>
<wire x1="19.787" y1="-12.319" x2="19.787" y2="-17.018" width="0.1524" layer="21"/>
<wire x1="19.787" y1="-12.319" x2="20.422" y2="-11.684" width="0.1524" layer="21" curve="-90"/>
<wire x1="6.706" y1="-13.97" x2="8.992" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="8.992" y1="-13.97" x2="8.992" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="8.992" y1="-16.256" x2="8.611" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="8.611" y1="-16.256" x2="8.611" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="8.611" y1="-14.351" x2="7.087" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="7.087" y1="-14.351" x2="7.087" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="7.087" y1="-16.256" x2="6.706" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="6.706" y1="-16.256" x2="6.706" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="10.643" y1="-13.97" x2="12.929" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="12.929" y1="-13.97" x2="12.929" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="12.929" y1="-16.256" x2="12.548" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="12.548" y1="-16.256" x2="12.548" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="12.548" y1="-14.351" x2="11.024" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="11.024" y1="-14.351" x2="11.024" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="11.024" y1="-16.256" x2="10.643" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="10.643" y1="-16.256" x2="10.643" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-13.36" y1="-13.97" x2="-11.074" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-11.074" y1="-13.97" x2="-11.074" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-11.074" y1="-16.256" x2="-11.455" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-11.455" y1="-16.256" x2="-11.455" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-11.455" y1="-14.351" x2="-12.979" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-12.979" y1="-14.351" x2="-12.979" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-12.979" y1="-16.256" x2="-13.36" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-13.36" y1="-16.256" x2="-13.36" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-9.423" y1="-13.97" x2="-7.137" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-7.137" y1="-13.97" x2="-7.137" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-7.137" y1="-16.256" x2="-7.518" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-7.518" y1="-16.256" x2="-7.518" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-7.518" y1="-14.351" x2="-9.042" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-9.042" y1="-14.351" x2="-9.042" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-9.042" y1="-16.256" x2="-9.423" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-9.423" y1="-16.256" x2="-9.423" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-5.486" y1="-13.97" x2="-3.2" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-3.2" y1="-13.97" x2="-3.2" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-3.2" y1="-16.256" x2="-3.581" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-3.581" y1="-16.256" x2="-3.581" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-3.581" y1="-14.351" x2="-5.105" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-5.105" y1="-14.351" x2="-5.105" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-5.105" y1="-16.256" x2="-5.486" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-5.486" y1="-16.256" x2="-5.486" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-1.295" y1="-13.97" x2="0.991" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="0.991" y1="-13.97" x2="0.991" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="0.991" y1="-16.256" x2="0.61" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="0.61" y1="-16.256" x2="0.61" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="0.61" y1="-14.351" x2="-0.914" y2="-14.351" width="0.1524" layer="21"/>
<wire x1="-0.914" y1="-14.351" x2="-0.914" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-0.914" y1="-16.256" x2="-1.295" y2="-16.256" width="0.1524" layer="21"/>
<wire x1="-1.295" y1="-16.256" x2="-1.295" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-19.025" y1="-12.192" x2="-19.025" y2="-17.526" width="0.1524" layer="21"/>
<wire x1="-19.025" y1="-17.526" x2="-19.279" y2="-17.526" width="0.1524" layer="21"/>
<wire x1="19.025" y1="-12.192" x2="19.025" y2="-17.526" width="0.1524" layer="21"/>
<wire x1="19.025" y1="-17.526" x2="-19.025" y2="-17.526" width="0.1524" layer="21"/>
<text x="13.6525" y="5.3975" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-22.5425" y="5.3975" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<text x="-19.685" y="3.4925" size="1.27" layer="21" ratio="10">13</text>
<text x="18.0975" y="3.4925" size="1.27" layer="21" ratio="10">1</text>
<text x="-18.812" y="-2.682" size="1.27" layer="21" ratio="10">25</text>
<text x="17.153" y="-2.38" size="1.27" layer="21" ratio="10">14</text>
<text x="-18.319" y="-15.104" size="1.27" layer="21" ratio="10">M25</text>
<rectangle x1="-20.3" y1="-8.92" x2="20.275" y2="-8.158" layer="21"/>
<rectangle x1="-26.55" y1="-11.684" x2="26.525" y2="-11.04" layer="21"/>
<hole x="-22.57" y="0" drill="1.7"/>
<hole x="22.57" y="0" drill="1.7"/>
<smd name="1" x="16.56" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="14" x="15.18" y="-1.975" dx="1" dy="2.8" layer="1"/>
<smd name="2" x="13.8" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="3" x="11.04" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="4" x="8.28" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="5" x="5.52" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="6" x="2.76" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="7" x="0" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="8" x="-2.76" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="9" x="-5.52" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="10" x="-8.28" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="11" x="-11.04" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="12" x="-13.8" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="13" x="-16.56" y="1.975" dx="1" dy="2.8" layer="1"/>
<smd name="S@2" x="-23.445" y="-2.625" dx="1" dy="1" layer="1"/>
<smd name="S@1" x="23.445" y="-2.65" dx="1" dy="1" layer="1"/>
<wire x1="-26.6475" y1="-8.1" x2="26.6475" y2="-8.1" width="0" layer="20"/>
<polygon width="0.2" layer="1">
<vertex x="-26.3275" y="-7.9375"/>
<vertex x="-20.52" y="-7.9375"/>
<vertex x="-20.52" y="-1.5875"/>
<vertex x="-22.5175" y="-1.5875" curve="-180"/>
<vertex x="-22.5175" y="1.5875"/>
<vertex x="-20.495" y="1.5875"/>
<vertex x="-20.495" y="3.325"/>
<vertex x="-26.3275" y="3.325"/>
</polygon>
<polygon width="0.2" layer="1">
<vertex x="26.3275" y="-7.9375"/>
<vertex x="20.52" y="-7.9375"/>
<vertex x="20.52" y="-1.5875"/>
<vertex x="22.5175" y="-1.5875" curve="180"/>
<vertex x="22.5175" y="1.5875"/>
<vertex x="20.495" y="1.5875"/>
<vertex x="20.495" y="3.325"/>
<vertex x="26.3275" y="3.325"/>
</polygon>
<wire x1="26.47" y1="3.016" x2="-26.47" y2="3.016" width="0.1524" layer="21"/>
<smd name="15" x="12.42" y="-1.975" dx="1" dy="2.8" layer="1"/>
<smd name="16" x="9.66" y="-1.975" dx="1" dy="2.8" layer="1"/>
<smd name="17" x="6.9" y="-1.975" dx="1" dy="2.8" layer="1"/>
<smd name="18" x="4.14" y="-1.975" dx="1" dy="2.8" layer="1"/>
<smd name="19" x="1.38" y="-1.975" dx="1" dy="2.8" layer="1"/>
<smd name="20" x="-1.38" y="-1.975" dx="1" dy="2.8" layer="1"/>
<smd name="21" x="-4.14" y="-1.975" dx="1" dy="2.8" layer="1"/>
<smd name="22" x="-6.9" y="-1.975" dx="1" dy="2.8" layer="1"/>
<smd name="23" x="-9.66" y="-1.975" dx="1" dy="2.8" layer="1"/>
<smd name="24" x="-12.42" y="-1.975" dx="1" dy="2.8" layer="1"/>
<smd name="25" x="-15.18" y="-1.975" dx="1" dy="2.8" layer="1"/>
</package>
</packages>
<symbols>
<symbol name="M25">
<wire x1="1.27" y1="12.7" x2="2.54" y2="12.7" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="12.7" x2="-1.27" y2="12.7" width="0.6096" layer="94"/>
<wire x1="1.27" y1="7.62" x2="2.54" y2="7.62" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="10.16" x2="-1.27" y2="10.16" width="0.6096" layer="94"/>
<wire x1="1.27" y1="2.54" x2="2.54" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="7.62" x2="-1.27" y2="7.62" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="5.08" x2="-1.27" y2="5.08" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-7.62" x2="2.54" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="2.54" x2="-1.27" y2="2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-12.7" x2="2.54" y2="-12.7" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="0" x2="-1.27" y2="0" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-17.78" x2="2.54" y2="-17.78" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="-2.54" x2="-1.27" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="10.16" x2="2.54" y2="10.16" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="-5.08" x2="-1.27" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="1.27" y1="5.08" x2="2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="-7.62" x2="-1.27" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="-10.16" x2="-1.27" y2="-10.16" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-5.08" x2="2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="-12.7" x2="-1.27" y2="-12.7" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-10.16" x2="2.54" y2="-10.16" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="-15.24" x2="-1.27" y2="-15.24" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-15.24" x2="2.54" y2="-15.24" width="0.6096" layer="94"/>
<wire x1="2.5226" y1="-20.8718" x2="4.0637" y2="-19.6312" width="0.4064" layer="94" curve="102.322193" cap="flat"/>
<wire x1="2.5226" y1="-20.8718" x2="-3.0654" y2="-19.6494" width="0.4064" layer="94"/>
<wire x1="-4.064" y1="-18.4088" x2="-3.0654" y2="-19.6494" width="0.4064" layer="94" curve="77.657889"/>
<wire x1="-4.064" y1="13.3288" x2="-4.064" y2="-18.4088" width="0.4064" layer="94"/>
<wire x1="-4.064" y1="13.3288" x2="-3.0654" y2="14.5694" width="0.4064" layer="94" curve="-77.657889"/>
<wire x1="4.064" y1="14.5512" x2="4.064" y2="-19.6312" width="0.4064" layer="94"/>
<wire x1="2.5226" y1="15.7918" x2="-3.0654" y2="14.5694" width="0.4064" layer="94"/>
<wire x1="2.5226" y1="15.7919" x2="4.064" y2="14.5512" width="0.4064" layer="94" curve="-102.337599" cap="flat"/>
<text x="-3.81" y="-23.495" size="1.778" layer="96">&gt;VALUE</text>
<text x="-3.81" y="16.51" size="1.778" layer="95">&gt;NAME</text>
<pin name="1" x="7.62" y="12.7" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="14" x="-7.62" y="12.7" visible="pad" length="middle" direction="pas"/>
<pin name="2" x="7.62" y="10.16" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="15" x="-7.62" y="10.16" visible="pad" length="middle" direction="pas"/>
<pin name="3" x="7.62" y="7.62" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="16" x="-7.62" y="7.62" visible="pad" length="middle" direction="pas"/>
<pin name="4" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="17" x="-7.62" y="5.08" visible="pad" length="middle" direction="pas"/>
<pin name="5" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="18" x="-7.62" y="2.54" visible="pad" length="middle" direction="pas"/>
<pin name="6" x="7.62" y="0" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="19" x="-7.62" y="0" visible="pad" length="middle" direction="pas"/>
<pin name="7" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="20" x="-7.62" y="-2.54" visible="pad" length="middle" direction="pas"/>
<pin name="8" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="21" x="-7.62" y="-5.08" visible="pad" length="middle" direction="pas"/>
<pin name="9" x="7.62" y="-7.62" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="22" x="-7.62" y="-7.62" visible="pad" length="middle" direction="pas"/>
<pin name="10" x="7.62" y="-10.16" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="23" x="-7.62" y="-10.16" visible="pad" length="middle" direction="pas"/>
<pin name="11" x="7.62" y="-12.7" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="24" x="-7.62" y="-12.7" visible="pad" length="middle" direction="pas"/>
<pin name="12" x="7.62" y="-15.24" visible="pad" length="middle" direction="pas" rot="R180"/>
<pin name="25" x="-7.62" y="-15.24" visible="pad" length="middle" direction="pas"/>
<pin name="13" x="7.62" y="-17.78" visible="pad" length="middle" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="M25" prefix="X" uservalue="yes">
<description>&lt;b&gt;SUB-D&lt;/b&gt;</description>
<gates>
<gate name="-1" symbol="M25" x="0" y="0"/>
</gates>
<devices>
<device name="D" package="M25D">
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
<device name="H" package="M25H">
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
<device name="HP" package="M25HP">
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
<device name="V" package="M25V">
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
<device name="VP" package="M25VP">
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
<device name="VB" package="M25VB">
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
<device name="-182" package="182-025-MALE">
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
<device name="SMT" package="M25SMT">
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
<part name="LED_1" library="led" deviceset="LED" device="B155"/>
<part name="ON" library="switch-misc" deviceset="9450-1" device=""/>
<part name="BRAKE_SWITCH" library="switch-misc" deviceset="9450-1" device=""/>
<part name="MOTOR_SWITCH" library="switch-misc" deviceset="9450-1" device=""/>
<part name="STEER_SWITCH" library="switch-misc" deviceset="9450-1" device=""/>
<part name="R8" library="resistor" deviceset="R-US_" device="0617/17" value="1k"/>
<part name="R9" library="resistor" deviceset="R-US_" device="0617/17" value="10000"/>
<part name="P+3" library="supply1" deviceset="+5V" device="" value="12volt"/>
<part name="GND9" library="supply1" deviceset="GND" device=""/>
<part name="R10" library="resistor" deviceset="R-US_" device="0617/17" value="200"/>
<part name="R16" library="resistor" deviceset="R-US_" device="0617/17" value="200"/>
<part name="R17" library="resistor" deviceset="R-US_" device="0617/17" value="200"/>
<part name="P+9" library="supply1" deviceset="+5V" device=""/>
<part name="GND13" library="supply1" deviceset="GND" device=""/>
<part name="P+11" library="supply1" deviceset="+5V" device=""/>
<part name="P+12" library="supply1" deviceset="+5V" device=""/>
<part name="REVERSE" library="led" deviceset="LED" device="B155"/>
<part name="R21" library="resistor" deviceset="R-US_" device="0617/17" value="1k"/>
<part name="GND17" library="supply1" deviceset="GND" device=""/>
<part name="ON_SWITCH" library="led" deviceset="LED" device="B155"/>
<part name="CRUISE_SWITCH1" library="switch-misc" deviceset="9450-1" device=""/>
<part name="R13" library="resistor" deviceset="R-US_" device="0617/17" value="220"/>
<part name="P+1" library="supply1" deviceset="+5V" device=""/>
<part name="STOP_M_SWITCH" library="switch-misc" deviceset="9450-1" device=""/>
<part name="R23" library="resistor" deviceset="R-US_" device="0617/17" value="220"/>
<part name="P+4" library="supply1" deviceset="+5V" device=""/>
<part name="LED_8" library="led" deviceset="LED" device="B155"/>
<part name="R2" library="resistor" deviceset="R-US_" device="0617/17" value="1k"/>
<part name="GND3" library="supply1" deviceset="GND" device=""/>
<part name="LED_3" library="led" deviceset="LED" device="B155"/>
<part name="R3" library="resistor" deviceset="R-US_" device="0617/17" value="1k"/>
<part name="GND4" library="supply1" deviceset="GND" device=""/>
<part name="LED_4" library="led" deviceset="LED" device="B155"/>
<part name="R4" library="resistor" deviceset="R-US_" device="0617/17" value="1k"/>
<part name="GND5" library="supply1" deviceset="GND" device=""/>
<part name="LED_5" library="led" deviceset="LED" device="B155"/>
<part name="R5" library="resistor" deviceset="R-US_" device="0617/17" value="1k"/>
<part name="GND6" library="supply1" deviceset="GND" device=""/>
<part name="LED_6" library="led" deviceset="LED" device="B155"/>
<part name="R6" library="resistor" deviceset="R-US_" device="0617/17" value="1k"/>
<part name="GND7" library="supply1" deviceset="GND" device=""/>
<part name="LED_7" library="led" deviceset="LED" device="B155"/>
<part name="R7" library="resistor" deviceset="R-US_" device="0617/17" value="1k"/>
<part name="GND8" library="supply1" deviceset="GND" device=""/>
<part name="TO_SERVOCITY_POWER" library="con-molex" deviceset="22-23-2021" device=""/>
<part name="X1" library="con-subd" deviceset="M25" device="-182"/>
<part name="GND1" library="supply1" deviceset="GND" device=""/>
<part name="P+2" library="supply1" deviceset="+5V" device=""/>
<part name="CRUISE_LED" library="led" deviceset="LED" device="B155"/>
<part name="R1" library="resistor" deviceset="R-US_" device="0617/17" value="1k"/>
<part name="GND2" library="supply1" deviceset="GND" device=""/>
<part name="STOP_LED" library="led" deviceset="LED" device="B155"/>
<part name="R12" library="resistor" deviceset="R-US_" device="0617/17" value="1k"/>
<part name="GND12" library="supply1" deviceset="GND" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="LED_1" gate="G$1" x="-325.12" y="119.38" rot="R270"/>
<instance part="ON" gate="1" x="-386.08" y="137.16"/>
<instance part="BRAKE_SWITCH" gate="1" x="-243.84" y="182.88"/>
<instance part="MOTOR_SWITCH" gate="1" x="-274.32" y="182.88"/>
<instance part="STEER_SWITCH" gate="1" x="-294.64" y="190.5"/>
<instance part="R8" gate="G$1" x="-342.9" y="119.38"/>
<instance part="R9" gate="G$1" x="-386.08" y="91.44" rot="R90"/>
<instance part="P+3" gate="1" x="-383.54" y="160.02"/>
<instance part="GND9" gate="1" x="-358.14" y="119.38" rot="R270"/>
<instance part="R10" gate="G$1" x="-274.32" y="165.1" rot="R90"/>
<instance part="R16" gate="G$1" x="-243.84" y="167.64" rot="R90"/>
<instance part="R17" gate="G$1" x="-294.64" y="175.26" rot="R90"/>
<instance part="P+9" gate="1" x="-266.7" y="190.5"/>
<instance part="GND13" gate="1" x="-386.08" y="73.66"/>
<instance part="P+11" gate="1" x="-241.3" y="200.66"/>
<instance part="P+12" gate="1" x="-292.1" y="208.28"/>
<instance part="REVERSE" gate="G$1" x="-200.66" y="139.7" rot="R90"/>
<instance part="R21" gate="G$1" x="-182.88" y="139.7" rot="R180"/>
<instance part="GND17" gate="1" x="-167.64" y="139.7" rot="R90"/>
<instance part="ON_SWITCH" gate="G$1" x="-386.08" y="109.22"/>
<instance part="CRUISE_SWITCH1" gate="1" x="-314.96" y="20.32" rot="R90"/>
<instance part="R13" gate="G$1" x="-302.26" y="20.32" rot="R180"/>
<instance part="P+1" gate="1" x="-330.2" y="22.86" rot="R90"/>
<instance part="STOP_M_SWITCH" gate="1" x="-314.96" y="45.72" rot="R90"/>
<instance part="R23" gate="G$1" x="-302.26" y="45.72" rot="R180"/>
<instance part="P+4" gate="1" x="-330.2" y="48.26" rot="R90"/>
<instance part="LED_8" gate="G$1" x="-297.18" y="66.04" rot="R270"/>
<instance part="R2" gate="G$1" x="-314.96" y="66.04"/>
<instance part="GND3" gate="1" x="-330.2" y="66.04" rot="R270"/>
<instance part="LED_3" gate="G$1" x="-325.12" y="132.08" rot="R270"/>
<instance part="R3" gate="G$1" x="-342.9" y="132.08"/>
<instance part="GND4" gate="1" x="-358.14" y="132.08" rot="R270"/>
<instance part="LED_4" gate="G$1" x="-325.12" y="142.24" rot="R90"/>
<instance part="R4" gate="G$1" x="-342.9" y="142.24"/>
<instance part="GND5" gate="1" x="-358.14" y="142.24" rot="R270"/>
<instance part="LED_5" gate="G$1" x="-325.12" y="152.4" rot="R270"/>
<instance part="R5" gate="G$1" x="-342.9" y="152.4"/>
<instance part="GND6" gate="1" x="-358.14" y="152.4" rot="R270"/>
<instance part="LED_6" gate="G$1" x="-325.12" y="162.56" rot="R270"/>
<instance part="R6" gate="G$1" x="-342.9" y="162.56"/>
<instance part="GND7" gate="1" x="-358.14" y="162.56" rot="R270"/>
<instance part="LED_7" gate="G$1" x="-297.18" y="78.74" rot="R270"/>
<instance part="R7" gate="G$1" x="-317.5" y="78.74"/>
<instance part="GND8" gate="1" x="-330.2" y="78.74" rot="R270"/>
<instance part="TO_SERVOCITY_POWER" gate="-1" x="-396.24" y="119.38" rot="MR0"/>
<instance part="X1" gate="-1" x="-276.86" y="99.06" rot="R90"/>
<instance part="GND1" gate="1" x="-251.46" y="83.82"/>
<instance part="P+2" gate="1" x="-264.16" y="81.28" rot="R180"/>
<instance part="CRUISE_LED" gate="G$1" x="-238.76" y="116.84" rot="R90"/>
<instance part="R1" gate="G$1" x="-220.98" y="116.84" rot="R180"/>
<instance part="GND2" gate="1" x="-205.74" y="116.84" rot="R90"/>
<instance part="STOP_LED" gate="G$1" x="-241.3" y="129.54" rot="R90"/>
<instance part="R12" gate="G$1" x="-223.52" y="129.54" rot="R180"/>
<instance part="GND12" gate="1" x="-208.28" y="129.54" rot="R90"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$16" class="0">
<segment>
<pinref part="LED_1" gate="G$1" pin="C"/>
<pinref part="R8" gate="G$1" pin="2"/>
<wire x1="-330.2" y1="119.38" x2="-337.82" y2="119.38" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="R8" gate="G$1" pin="1"/>
<pinref part="GND9" gate="1" pin="GND"/>
<wire x1="-347.98" y1="119.38" x2="-355.6" y2="119.38" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="R21" gate="G$1" pin="1"/>
<pinref part="GND17" gate="1" pin="GND"/>
<wire x1="-177.8" y1="139.7" x2="-170.18" y2="139.7" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="GND13" gate="1" pin="GND"/>
<pinref part="R9" gate="G$1" pin="1"/>
<wire x1="-386.08" y1="76.2" x2="-386.08" y2="86.36" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="R2" gate="G$1" pin="1"/>
<pinref part="GND3" gate="1" pin="GND"/>
<wire x1="-320.04" y1="66.04" x2="-327.66" y2="66.04" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="R3" gate="G$1" pin="1"/>
<pinref part="GND4" gate="1" pin="GND"/>
<wire x1="-347.98" y1="132.08" x2="-355.6" y2="132.08" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="R4" gate="G$1" pin="1"/>
<pinref part="GND5" gate="1" pin="GND"/>
<wire x1="-347.98" y1="142.24" x2="-355.6" y2="142.24" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="R5" gate="G$1" pin="1"/>
<pinref part="GND6" gate="1" pin="GND"/>
<wire x1="-347.98" y1="152.4" x2="-355.6" y2="152.4" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="R6" gate="G$1" pin="1"/>
<pinref part="GND7" gate="1" pin="GND"/>
<wire x1="-347.98" y1="162.56" x2="-355.6" y2="162.56" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="R7" gate="G$1" pin="1"/>
<pinref part="GND8" gate="1" pin="GND"/>
<wire x1="-322.58" y1="78.74" x2="-327.66" y2="78.74" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="X1" gate="-1" pin="25"/>
<pinref part="GND1" gate="1" pin="GND"/>
<wire x1="-261.62" y1="91.44" x2="-251.46" y2="86.36" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="R1" gate="G$1" pin="1"/>
<pinref part="GND2" gate="1" pin="GND"/>
<wire x1="-215.9" y1="116.84" x2="-208.28" y2="116.84" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="R12" gate="G$1" pin="1"/>
<pinref part="GND12" gate="1" pin="GND"/>
<wire x1="-218.44" y1="129.54" x2="-210.82" y2="129.54" width="0.1524" layer="91"/>
</segment>
</net>
<net name="+5V" class="0">
<segment>
<pinref part="P+3" gate="1" pin="+5V"/>
<pinref part="ON" gate="1" pin="S"/>
<wire x1="-383.54" y1="157.48" x2="-383.54" y2="142.24" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="P+9" gate="1" pin="+5V"/>
<pinref part="MOTOR_SWITCH" gate="1" pin="S"/>
<wire x1="-266.7" y1="187.96" x2="-271.78" y2="187.96" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="P+11" gate="1" pin="+5V"/>
<pinref part="BRAKE_SWITCH" gate="1" pin="S"/>
<wire x1="-241.3" y1="198.12" x2="-241.3" y2="187.96" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="P+12" gate="1" pin="+5V"/>
<pinref part="STEER_SWITCH" gate="1" pin="S"/>
<wire x1="-292.1" y1="205.74" x2="-292.1" y2="195.58" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="P+1" gate="1" pin="+5V"/>
<pinref part="CRUISE_SWITCH1" gate="1" pin="S"/>
<wire x1="-327.66" y1="22.86" x2="-320.04" y2="22.86" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="P+4" gate="1" pin="+5V"/>
<pinref part="STOP_M_SWITCH" gate="1" pin="S"/>
<wire x1="-327.66" y1="48.26" x2="-320.04" y2="48.26" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="X1" gate="-1" pin="24"/>
<pinref part="P+2" gate="1" pin="+5V"/>
<wire x1="-264.16" y1="91.44" x2="-264.16" y2="83.82" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$27" class="0">
<segment>
<pinref part="MOTOR_SWITCH" gate="1" pin="P"/>
<pinref part="R10" gate="G$1" pin="2"/>
<wire x1="-274.32" y1="177.8" x2="-274.32" y2="170.18" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$28" class="0">
<segment>
<pinref part="R10" gate="G$1" pin="1"/>
<pinref part="X1" gate="-1" pin="8"/>
<wire x1="-271.78" y1="106.68" x2="-271.78" y2="149.86" width="0.1524" layer="91"/>
<wire x1="-271.78" y1="149.86" x2="-274.32" y2="149.86" width="0.1524" layer="91"/>
<wire x1="-274.32" y1="149.86" x2="-274.32" y2="160.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$29" class="0">
<segment>
<pinref part="BRAKE_SWITCH" gate="1" pin="P"/>
<pinref part="R16" gate="G$1" pin="2"/>
<wire x1="-243.84" y1="177.8" x2="-243.84" y2="172.72" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$30" class="0">
<segment>
<pinref part="STEER_SWITCH" gate="1" pin="P"/>
<pinref part="R17" gate="G$1" pin="2"/>
<wire x1="-294.64" y1="185.42" x2="-294.64" y2="180.34" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$32" class="0">
<segment>
<pinref part="R16" gate="G$1" pin="1"/>
<pinref part="X1" gate="-1" pin="7"/>
<wire x1="-274.32" y1="106.68" x2="-274.32" y2="144.78" width="0.1524" layer="91"/>
<wire x1="-274.32" y1="144.78" x2="-243.84" y2="144.78" width="0.1524" layer="91"/>
<wire x1="-243.84" y1="144.78" x2="-243.84" y2="162.56" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$22" class="0">
<segment>
<pinref part="REVERSE" gate="G$1" pin="A"/>
<wire x1="-210.82" y1="139.7" x2="-208.28" y2="139.7" width="0.1524" layer="91"/>
<pinref part="X1" gate="-1" pin="11"/>
<wire x1="-208.28" y1="139.7" x2="-203.2" y2="139.7" width="0.1524" layer="91"/>
<wire x1="-264.16" y1="106.68" x2="-264.16" y2="139.7" width="0.1524" layer="91"/>
<wire x1="-264.16" y1="139.7" x2="-210.82" y2="139.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="REVERSE" class="0">
<segment>
<pinref part="REVERSE" gate="G$1" pin="C"/>
<pinref part="R21" gate="G$1" pin="2"/>
<wire x1="-195.58" y1="139.7" x2="-187.96" y2="139.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<pinref part="ON" gate="1" pin="P"/>
<wire x1="-386.08" y1="119.38" x2="-386.08" y2="132.08" width="0.1524" layer="91"/>
<junction x="-386.08" y="119.38"/>
<pinref part="ON_SWITCH" gate="G$1" pin="A"/>
<wire x1="-386.08" y1="111.76" x2="-386.08" y2="119.38" width="0.1524" layer="91"/>
<pinref part="TO_SERVOCITY_POWER" gate="-1" pin="S"/>
<wire x1="-393.7" y1="119.38" x2="-386.08" y2="119.38" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<pinref part="R9" gate="G$1" pin="2"/>
<pinref part="ON_SWITCH" gate="G$1" pin="C"/>
<wire x1="-386.08" y1="96.52" x2="-386.08" y2="104.14" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$19" class="0">
<segment>
<pinref part="R13" gate="G$1" pin="1"/>
<pinref part="X1" gate="-1" pin="17"/>
<wire x1="-281.94" y1="91.44" x2="-281.94" y2="20.32" width="0.1524" layer="91"/>
<wire x1="-281.94" y1="20.32" x2="-297.18" y2="20.32" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$34" class="0">
<segment>
<pinref part="R13" gate="G$1" pin="2"/>
<pinref part="CRUISE_SWITCH1" gate="1" pin="P"/>
<wire x1="-309.88" y1="20.32" x2="-307.34" y2="20.32" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$35" class="0">
<segment>
<pinref part="R23" gate="G$1" pin="1"/>
<pinref part="X1" gate="-1" pin="18"/>
<wire x1="-279.4" y1="91.44" x2="-279.4" y2="45.72" width="0.1524" layer="91"/>
<wire x1="-279.4" y1="45.72" x2="-297.18" y2="45.72" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$37" class="0">
<segment>
<pinref part="R23" gate="G$1" pin="2"/>
<pinref part="STOP_M_SWITCH" gate="1" pin="P"/>
<wire x1="-309.88" y1="45.72" x2="-307.34" y2="45.72" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="LED_1" gate="G$1" pin="A"/>
<wire x1="-322.58" y1="119.38" x2="-279.4" y2="119.38" width="0.1524" layer="91"/>
<pinref part="X1" gate="-1" pin="5"/>
<wire x1="-279.4" y1="119.38" x2="-279.4" y2="106.68" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="LED_8" gate="G$1" pin="C"/>
<pinref part="R2" gate="G$1" pin="2"/>
<wire x1="-302.26" y1="66.04" x2="-309.88" y2="66.04" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="LED_3" gate="G$1" pin="C"/>
<pinref part="R3" gate="G$1" pin="2"/>
<wire x1="-330.2" y1="132.08" x2="-337.82" y2="132.08" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="LED_3" gate="G$1" pin="A"/>
<pinref part="X1" gate="-1" pin="4"/>
<wire x1="-281.94" y1="106.68" x2="-281.94" y2="132.08" width="0.1524" layer="91"/>
<wire x1="-281.94" y1="132.08" x2="-322.58" y2="132.08" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="LED_4" gate="G$1" pin="C"/>
<pinref part="R4" gate="G$1" pin="2"/>
<wire x1="-320.04" y1="142.24" x2="-337.82" y2="142.24" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="LED_4" gate="G$1" pin="A"/>
<pinref part="X1" gate="-1" pin="3"/>
<wire x1="-284.48" y1="106.68" x2="-284.48" y2="142.24" width="0.1524" layer="91"/>
<wire x1="-284.48" y1="142.24" x2="-327.66" y2="142.24" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="LED_5" gate="G$1" pin="C"/>
<pinref part="R5" gate="G$1" pin="2"/>
<wire x1="-330.2" y1="152.4" x2="-337.82" y2="152.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="LED_5" gate="G$1" pin="A"/>
<pinref part="X1" gate="-1" pin="2"/>
<wire x1="-287.02" y1="106.68" x2="-287.02" y2="152.4" width="0.1524" layer="91"/>
<wire x1="-287.02" y1="152.4" x2="-322.58" y2="152.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<pinref part="LED_6" gate="G$1" pin="C"/>
<pinref part="R6" gate="G$1" pin="2"/>
<wire x1="-330.2" y1="162.56" x2="-337.82" y2="162.56" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="LED_6" gate="G$1" pin="A"/>
<pinref part="X1" gate="-1" pin="1"/>
<wire x1="-289.56" y1="106.68" x2="-289.56" y2="162.56" width="0.1524" layer="91"/>
<wire x1="-289.56" y1="162.56" x2="-322.58" y2="162.56" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="LED_7" gate="G$1" pin="C"/>
<pinref part="R7" gate="G$1" pin="2"/>
<wire x1="-302.26" y1="78.74" x2="-312.42" y2="78.74" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="LED_8" gate="G$1" pin="A"/>
<wire x1="-294.64" y1="66.04" x2="-284.48" y2="66.04" width="0.1524" layer="91"/>
<pinref part="X1" gate="-1" pin="16"/>
<wire x1="-284.48" y1="91.44" x2="-284.48" y2="66.04" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$20" class="0">
<segment>
<pinref part="LED_7" gate="G$1" pin="A"/>
<wire x1="-287.02" y1="78.74" x2="-294.64" y2="78.74" width="0.1524" layer="91"/>
<pinref part="X1" gate="-1" pin="15"/>
<wire x1="-287.02" y1="91.44" x2="-287.02" y2="78.74" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$1" class="0">
<segment>
<pinref part="X1" gate="-1" pin="6"/>
<wire x1="-276.86" y1="106.68" x2="-276.86" y2="157.48" width="0.1524" layer="91"/>
<wire x1="-276.86" y1="157.48" x2="-284.48" y2="157.48" width="0.1524" layer="91"/>
<wire x1="-284.48" y1="157.48" x2="-284.48" y2="167.64" width="0.1524" layer="91"/>
<wire x1="-284.48" y1="167.64" x2="-294.64" y2="167.64" width="0.1524" layer="91"/>
<pinref part="R17" gate="G$1" pin="1"/>
<wire x1="-294.64" y1="167.64" x2="-294.64" y2="170.18" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="CRUISE_LED" gate="G$1" pin="C"/>
<pinref part="R1" gate="G$1" pin="2"/>
<wire x1="-233.68" y1="116.84" x2="-226.06" y2="116.84" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<pinref part="CRUISE_LED" gate="G$1" pin="A"/>
<pinref part="X1" gate="-1" pin="9"/>
<wire x1="-269.24" y1="106.68" x2="-269.24" y2="116.84" width="0.1524" layer="91"/>
<wire x1="-269.24" y1="116.84" x2="-241.3" y2="116.84" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$25" class="0">
<segment>
<pinref part="STOP_LED" gate="G$1" pin="C"/>
<pinref part="R12" gate="G$1" pin="2"/>
<wire x1="-236.22" y1="129.54" x2="-228.6" y2="129.54" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$26" class="0">
<segment>
<pinref part="STOP_LED" gate="G$1" pin="A"/>
<wire x1="-243.84" y1="129.54" x2="-251.46" y2="129.54" width="0.1524" layer="91"/>
<pinref part="X1" gate="-1" pin="10"/>
<wire x1="-266.7" y1="106.68" x2="-266.7" y2="129.54" width="0.1524" layer="91"/>
<wire x1="-266.7" y1="129.54" x2="-254" y2="129.54" width="0.1524" layer="91"/>
<wire x1="-251.46" y1="129.54" x2="-254" y2="129.54" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
