<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.4">
<drawing>
<settings>
<setting alwaysvectorfont="yes"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.05" unitdist="inch" unit="inch" style="lines" multiple="1" display="yes" altdistance="0.01" altunitdist="inch" altunit="inch"/>
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
<library name="con-faston">
<description>&lt;b&gt;FASTON Connector&lt;/b&gt; Printed Circuit Board Tabs&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="62221">
<description>&lt;b&gt;TAB, FSSTON, DUAL STAKE, .187 SERIES&lt;/b&gt;&lt;p&gt;
Source: Tyco Electronics Corporation .. ENG_CD_62221_R.pdf&lt;br&gt;
http://search.digikey.com/scripts/DkSearch/dksus.dll?Detail&amp;name=A24734CT-ND&lt;br&gt;</description>
<wire x1="2.54" y1="1.4605" x2="2.54" y2="-1.4605" width="0" layer="46"/>
<wire x1="2.54" y1="-1.4605" x2="-2.54" y2="-1.4605" width="0" layer="46"/>
<wire x1="-2.54" y1="-1.4605" x2="-2.54" y2="1.4605" width="0" layer="46"/>
<wire x1="-2.54" y1="1.4605" x2="2.54" y2="1.4605" width="0" layer="46"/>
<wire x1="-3.6195" y1="2.921" x2="-3.6195" y2="-2.921" width="0.508" layer="51"/>
<wire x1="3.6195" y1="-2.921" x2="3.6195" y2="2.921" width="0.508" layer="51"/>
<wire x1="-3.6" y1="3.075" x2="3.625" y2="3.075" width="0.2032" layer="51"/>
<wire x1="3.6" y1="-3.075" x2="-3.625" y2="-3.075" width="0.2032" layer="51"/>
<wire x1="-2.2352" y1="1.2192" x2="-2.2352" y2="-1.2192" width="0.2032" layer="51"/>
<wire x1="2.2352" y1="-1.2192" x2="2.2352" y2="1.2192" width="0.2032" layer="51"/>
<wire x1="-2.2352" y1="1.2192" x2="2.2352" y2="1.2192" width="0.2032" layer="51"/>
<wire x1="2.2352" y1="-1.2192" x2="-2.2352" y2="-1.2192" width="0.2032" layer="51"/>
<pad name="1" x="0" y="0" drill="2.921" diameter="5.08" shape="long"/>
<text x="-3.8608" y="3.6576" size="1.27" layer="25">&gt;NAME</text>
<text x="-3.8608" y="-5.08" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-2.032" y1="3.048" x2="2.032" y2="3.4544" layer="51"/>
<rectangle x1="-2.032" y1="-3.4544" x2="2.032" y2="-3.048" layer="51" rot="R180"/>
</package>
</packages>
<symbols>
<symbol name="PIN">
<text x="-2.54" y="1.27" size="1.778" layer="95">&gt;NAME</text>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<rectangle x1="0" y1="-0.508" x2="2.54" y2="0.508" layer="94"/>
<pin name="1" x="-5.08" y="0" visible="off" length="middle" direction="pas" swaplevel="1"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="PIN1-2" prefix="J">
<description>&lt;b&gt;CONN TAB FASTON BRASS .187 PCB&lt;/b&gt;&lt;p&gt;
TAB, FSSTON, DUAL STAKE, .187 SERIES&lt;br&gt;
Digi-Key Part Number: A24734CT-ND&lt;br&gt;
Source: Tyco Electronics Corporation .. ENG_CD_62221_R.pdf&lt;br&gt;
http://search.digikey.com/scripts/DkSearch/dksus.dll?Detail&amp;name=A24734CT-ND&lt;br&gt;</description>
<gates>
<gate name="G$1" symbol="PIN" x="0" y="0"/>
</gates>
<devices>
<device name="" package="62221">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="TYCO ELECTRONICS" constant="no"/>
<attribute name="MPN" value="62221-1" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="16H2318" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="battery">
<description>&lt;b&gt;Lithium Batteries and NC Accus&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="SL-150-1/2AA/PR">
<description>&lt;b&gt;LI BATTERY&lt;/b&gt; Sonnenschein</description>
<wire x1="9.525" y1="-7.366" x2="-12.192" y2="-7.366" width="0.1524" layer="21"/>
<wire x1="12.446" y1="-2.413" x2="12.446" y2="2.413" width="0.4064" layer="51"/>
<wire x1="-12.7" y1="-2.413" x2="-12.7" y2="2.413" width="0.4064" layer="51"/>
<wire x1="12.192" y1="1.905" x2="12.192" y2="3.556" width="0.1524" layer="21"/>
<wire x1="12.192" y1="-3.556" x2="12.192" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="12.192" y1="3.556" x2="11.303" y2="3.556" width="0.1524" layer="21"/>
<wire x1="11.303" y1="3.556" x2="9.906" y2="6.985" width="0.1524" layer="21"/>
<wire x1="12.192" y1="-3.556" x2="11.303" y2="-3.556" width="0.1524" layer="21"/>
<wire x1="11.303" y1="-3.556" x2="11.303" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="11.303" y1="-1.524" x2="11.303" y2="1.397" width="0.1524" layer="51"/>
<wire x1="11.303" y1="1.397" x2="11.303" y2="3.556" width="0.1524" layer="21"/>
<wire x1="-10.414" y1="0" x2="-8.89" y2="0" width="0.254" layer="21"/>
<wire x1="8.509" y1="-0.762" x2="8.509" y2="0.762" width="0.254" layer="21"/>
<wire x1="7.747" y1="0" x2="9.271" y2="0" width="0.254" layer="21"/>
<wire x1="9.906" y1="-6.985" x2="11.303" y2="-3.556" width="0.1524" layer="21"/>
<wire x1="9.525" y1="7.366" x2="-12.192" y2="7.366" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="2.54" x2="-1.905" y2="0" width="0.254" layer="21"/>
<wire x1="-5.08" y1="0" x2="-3.175" y2="0" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="0" x2="0" y2="0" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="0" x2="-1.905" y2="-2.54" width="0.254" layer="21"/>
<wire x1="-1.397" y1="1.524" x2="-0.381" y2="1.524" width="0.1524" layer="21"/>
<wire x1="-0.889" y1="2.032" x2="-0.889" y2="1.016" width="0.1524" layer="21"/>
<wire x1="12.192" y1="-1.905" x2="12.192" y2="1.905" width="0.1524" layer="51"/>
<wire x1="9.906" y1="6.985" x2="9.906" y2="-6.985" width="0.1524" layer="21"/>
<wire x1="-12.573" y1="-6.985" x2="-12.573" y2="-2.032" width="0.1524" layer="21"/>
<wire x1="-12.573" y1="6.985" x2="-12.573" y2="2.032" width="0.1524" layer="21"/>
<wire x1="9.525" y1="-7.366" x2="9.906" y2="-6.985" width="0.1524" layer="21" curve="90"/>
<wire x1="9.525" y1="7.366" x2="9.906" y2="6.985" width="0.1524" layer="21" curve="-90"/>
<wire x1="-12.573" y1="-6.985" x2="-12.192" y2="-7.366" width="0.1524" layer="21" curve="90"/>
<wire x1="-12.573" y1="6.985" x2="-12.192" y2="7.366" width="0.1524" layer="21" curve="-90"/>
<pad name="-" x="-12.7" y="0" drill="1.3208" diameter="3.1496" shape="octagon"/>
<pad name="+" x="12.446" y="0" drill="1.3208" diameter="3.1496" shape="octagon"/>
<text x="-12.7" y="8.001" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-8.763" y="4.191" size="1.27" layer="21" ratio="10">Lithium 3V</text>
<text x="-7.62" y="-5.715" size="1.27" layer="21" ratio="10">SL1/2PR</text>
<text x="-5.08" y="8.001" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-3.175" y1="-1.27" x2="-2.54" y2="1.27" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="1V2">
<wire x1="-0.635" y1="0.635" x2="-0.635" y2="0" width="0.4064" layer="94"/>
<wire x1="-2.54" y1="0" x2="-0.635" y2="0" width="0.1524" layer="94"/>
<wire x1="-0.635" y1="0" x2="-0.635" y2="-0.635" width="0.4064" layer="94"/>
<wire x1="0.635" y1="2.54" x2="0.635" y2="0" width="0.4064" layer="94"/>
<wire x1="0.635" y1="0" x2="2.54" y2="0" width="0.1524" layer="94"/>
<wire x1="0.635" y1="0" x2="0.635" y2="-2.54" width="0.4064" layer="94"/>
<text x="-1.27" y="3.175" size="1.778" layer="95">&gt;NAME</text>
<text x="-1.27" y="-5.08" size="1.778" layer="96">&gt;VALUE</text>
<pin name="+" x="5.08" y="0" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="-" x="-5.08" y="0" visible="pad" length="short" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="SL-150-1/2AA/PR" prefix="G">
<description>&lt;b&gt;LI BATTERY&lt;/b&gt; Sonnenschein</description>
<gates>
<gate name="G$1" symbol="1V2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SL-150-1/2AA/PR">
<connects>
<connect gate="G$1" pin="+" pad="+"/>
<connect gate="G$1" pin="-" pad="-"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="special">
<description>&lt;b&gt;Special Devices&lt;/b&gt;&lt;p&gt;
7-segment displays, switches, heatsinks, crystals, transformers, etc.&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="FUSE">
<description>&lt;B&gt;FUSE&lt;/B&gt;&lt;p&gt;
5 x 20 mm</description>
<wire x1="-11.43" y1="1.905" x2="-12.7" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-12.7" y1="-1.905" x2="-12.7" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-12.7" y1="-1.905" x2="-11.43" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-10.795" y1="2.54" x2="-10.795" y2="3.81" width="0.1524" layer="21"/>
<wire x1="-10.795" y1="3.81" x2="-6.985" y2="3.81" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="2.54" x2="-6.985" y2="3.81" width="0.1524" layer="21"/>
<wire x1="-10.795" y1="-2.54" x2="-10.795" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="-3.81" x2="-10.795" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="-3.81" x2="-6.985" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="1.905" x2="-5.08" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="1.905" x2="-5.08" y2="1.524" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-1.905" x2="-6.35" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="6.35" y1="1.905" x2="5.08" y2="1.905" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-1.905" x2="5.08" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-1.905" x2="6.35" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="6.985" y1="2.54" x2="6.985" y2="3.81" width="0.1524" layer="21"/>
<wire x1="6.985" y1="3.81" x2="10.795" y2="3.81" width="0.1524" layer="21"/>
<wire x1="10.795" y1="2.54" x2="10.795" y2="3.81" width="0.1524" layer="21"/>
<wire x1="6.985" y1="-2.54" x2="6.985" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="10.795" y1="-3.81" x2="6.985" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="10.795" y1="-3.81" x2="10.795" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="11.43" y1="1.905" x2="12.7" y2="1.905" width="0.1524" layer="21"/>
<wire x1="12.7" y1="1.905" x2="12.7" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="12.7" y1="-1.905" x2="11.43" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="2.794" x2="5.715" y2="2.794" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="-2.794" x2="5.715" y2="-2.794" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="1.524" x2="5.08" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="1.524" x2="-5.08" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-1.524" x2="5.08" y2="1.905" width="0.1524" layer="21"/>
<circle x="0" y="0" radius="0.127" width="0.1524" layer="21"/>
<pad name="1" x="-11.43" y="0" drill="1.3208" shape="long"/>
<pad name="2" x="11.43" y="0" drill="1.3208" shape="long"/>
<text x="-5.08" y="3.302" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-5.08" y="-5.08" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-12.065" y1="1.905" x2="-10.795" y2="3.175" layer="21"/>
<rectangle x1="-12.065" y1="-3.175" x2="-10.795" y2="-1.905" layer="21"/>
<rectangle x1="-11.43" y1="-2.54" x2="-10.795" y2="2.54" layer="51"/>
<rectangle x1="-6.985" y1="1.905" x2="-5.715" y2="3.175" layer="21"/>
<rectangle x1="-6.985" y1="-3.175" x2="-5.715" y2="-1.905" layer="21"/>
<rectangle x1="-6.985" y1="-2.54" x2="-6.35" y2="2.54" layer="21"/>
<rectangle x1="-10.795" y1="0.762" x2="-6.985" y2="2.54" layer="21"/>
<rectangle x1="-10.795" y1="-2.54" x2="-6.985" y2="-0.762" layer="21"/>
<rectangle x1="5.715" y1="1.905" x2="6.985" y2="3.175" layer="21"/>
<rectangle x1="5.715" y1="-3.175" x2="6.985" y2="-1.905" layer="21"/>
<rectangle x1="6.35" y1="-2.54" x2="6.985" y2="2.54" layer="21"/>
<rectangle x1="10.795" y1="1.905" x2="12.065" y2="3.175" layer="21"/>
<rectangle x1="10.795" y1="-3.175" x2="12.065" y2="-1.905" layer="21"/>
<rectangle x1="10.795" y1="-2.54" x2="11.43" y2="2.54" layer="51"/>
<rectangle x1="6.985" y1="0.762" x2="10.795" y2="2.54" layer="21"/>
<rectangle x1="6.985" y1="-2.54" x2="10.795" y2="-0.762" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="FUSE">
<wire x1="-5.08" y1="0" x2="-3.556" y2="1.524" width="0.254" layer="94"/>
<wire x1="0" y1="-1.524" x2="-2.54" y2="1.524" width="0.254" layer="94"/>
<wire x1="0.889" y1="-1.4986" x2="2.4892" y2="0" width="0.254" layer="94"/>
<wire x1="-3.5992" y1="1.4912" x2="-3.048" y2="1.7272" width="0.254" layer="94" curve="-46.337037" cap="flat"/>
<wire x1="-3.048" y1="1.7272" x2="-2.496" y2="1.491" width="0.254" layer="94" curve="-46.403624" cap="flat"/>
<wire x1="0.4572" y1="-1.778" x2="0.8965" y2="-1.4765" width="0.254" layer="94" curve="63.169357" cap="flat"/>
<wire x1="-0.0178" y1="-1.508" x2="0.4572" y2="-1.7778" width="0.254" layer="94" curve="64.986119" cap="flat"/>
<text x="-5.08" y="2.54" size="1.778" layer="95">&gt;NAME</text>
<text x="-5.08" y="-3.81" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-7.62" y="0" visible="off" length="short" direction="pas" function="dot" swaplevel="1"/>
<pin name="2" x="5.08" y="0" visible="off" length="short" direction="pas" function="dot" swaplevel="1" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="FUSE" prefix="F" uservalue="yes">
<description>&lt;B&gt;FUSE&lt;/B&gt;&lt;p&gt;
5 x 20 mm</description>
<gates>
<gate name="G$1" symbol="FUSE" x="12.7" y="0"/>
</gates>
<devices>
<device name="" package="FUSE">
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
<library name="switch">
<description>&lt;b&gt;Switches&lt;/b&gt;&lt;p&gt;
Marquardt, Siemens, C&amp;K, ITT, and others&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="MS243">
<description>&lt;b&gt;TOGGLE SWITCH&lt;/b&gt;</description>
<wire x1="-2.54" y1="2.286" x2="-2.54" y2="-3.81" width="0.1524" layer="51"/>
<wire x1="-2.54" y1="-4.826" x2="-2.032" y2="-5.334" width="0.1524" layer="21" curve="90"/>
<wire x1="2.032" y1="-5.334" x2="2.54" y2="-4.826" width="0.1524" layer="21" curve="90"/>
<wire x1="2.032" y1="2.794" x2="2.54" y2="2.286" width="0.1524" layer="51" curve="-90"/>
<wire x1="-2.54" y1="2.286" x2="-2.032" y2="2.794" width="0.1524" layer="51" curve="-90"/>
<wire x1="-2.032" y1="-5.334" x2="2.032" y2="-5.334" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-3.81" x2="2.54" y2="2.286" width="0.1524" layer="51"/>
<wire x1="-2.032" y1="2.794" x2="2.032" y2="2.794" width="0.1524" layer="51"/>
<wire x1="0" y1="2.667" x2="2.159" y2="1.524" width="0.1524" layer="51"/>
<wire x1="0" y1="2.667" x2="-2.159" y2="1.524" width="0.1524" layer="51"/>
<wire x1="2.667" y1="1.2446" x2="2.159" y2="1.524" width="0.1524" layer="51"/>
<wire x1="-2.667" y1="1.2446" x2="-2.159" y2="1.524" width="0.1524" layer="51"/>
<wire x1="-1.397" y1="-3.302" x2="-1.143" y2="-1.27" width="0.1524" layer="51"/>
<wire x1="-1.143" y1="-1.27" x2="1.143" y2="-1.27" width="0.1524" layer="51" curve="-180"/>
<wire x1="1.143" y1="-1.27" x2="1.397" y2="-3.302" width="0.1524" layer="51"/>
<wire x1="0" y1="0.381" x2="1.2892" y2="-2.3014" width="0.1524" layer="51" curve="-128.660783"/>
<wire x1="-1.2892" y1="-2.3014" x2="0" y2="0.381" width="0.1524" layer="51" curve="-128.660783"/>
<wire x1="-1.524" y1="2.6924" x2="1.524" y2="2.6924" width="2.1844" layer="29"/>
<wire x1="1.524" y1="4.5974" x2="1.524" y2="2.6924" width="2.1844" layer="30"/>
<wire x1="-1.524" y1="4.5974" x2="1.524" y2="4.5974" width="2.1844" layer="29"/>
<wire x1="-1.524" y1="4.5974" x2="-1.524" y2="2.6924" width="2.1844" layer="29"/>
<wire x1="-1.524" y1="4.5974" x2="-1.524" y2="2.6924" width="2.1844" layer="30"/>
<wire x1="-1.524" y1="4.5974" x2="1.524" y2="4.5974" width="2.1844" layer="30"/>
<wire x1="1.524" y1="4.5974" x2="1.524" y2="2.6924" width="2.1844" layer="29"/>
<wire x1="-1.524" y1="2.6924" x2="1.524" y2="2.6924" width="2.1844" layer="30"/>
<wire x1="-1.524" y1="0" x2="1.524" y2="0" width="2.1844" layer="29"/>
<wire x1="1.524" y1="-1.905" x2="-1.524" y2="-1.905" width="2.1844" layer="29"/>
<wire x1="1.524" y1="0" x2="1.524" y2="-1.905" width="2.1844" layer="29"/>
<wire x1="-1.524" y1="-1.905" x2="-1.524" y2="0" width="2.1844" layer="29"/>
<wire x1="-1.524" y1="0" x2="1.524" y2="0" width="2.1844" layer="30"/>
<wire x1="1.524" y1="-1.905" x2="-1.524" y2="-1.905" width="2.1844" layer="30"/>
<wire x1="-1.524" y1="-1.905" x2="-1.524" y2="0" width="2.1844" layer="30"/>
<wire x1="1.524" y1="0" x2="1.524" y2="-1.905" width="2.1844" layer="30"/>
<wire x1="0" y1="-5.207" x2="-1.27" y2="-4.572" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-4.572" x2="-2.413" y2="-3.937" width="0.1524" layer="51"/>
<wire x1="0" y1="-5.207" x2="1.27" y2="-4.572" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-4.572" x2="2.413" y2="-3.937" width="0.1524" layer="51"/>
<wire x1="3.556" y1="0.762" x2="3.556" y2="-3.302" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-3.81" x2="2.54" y2="-4.826" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-3.81" x2="-2.54" y2="-4.826" width="0.1524" layer="21"/>
<wire x1="3.556" y1="-3.302" x2="2.413" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="-3.556" y1="-3.302" x2="-2.413" y2="-3.937" width="0.1524" layer="21"/>
<wire x1="-3.556" y1="0.762" x2="-2.667" y2="1.2446" width="0.1524" layer="21"/>
<wire x1="-3.556" y1="-3.302" x2="-3.556" y2="0.762" width="0.1524" layer="21"/>
<wire x1="3.556" y1="0.762" x2="2.667" y2="1.2446" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.27" x2="2.54" y2="-1.27" width="0.1524" layer="51" curve="-180"/>
<wire x1="-2.54" y1="-1.27" x2="-1.3799" y2="-3.4025" width="0.1524" layer="51" curve="57.09406"/>
<wire x1="1.3799" y1="-3.4025" x2="2.54" y2="-1.27" width="0.1524" layer="51" curve="57.09406"/>
<wire x1="-1.397" y1="-3.302" x2="1.397" y2="-3.302" width="0.1524" layer="51" curve="180"/>
<pad name="1" x="0" y="-1.3462" drill="1.8034" diameter="2" shape="long"/>
<pad name="2" x="0" y="1.3462" drill="1.8034" diameter="2" shape="long"/>
<text x="4.445" y="-1.27" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="4.445" y="-3.81" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-1.27" y1="2.3368" x2="1.27" y2="4.2418" layer="42"/>
<rectangle x1="1.143" y1="0.3048" x2="2.667" y2="4.2418" layer="42"/>
<rectangle x1="-2.667" y1="0.3048" x2="-1.143" y2="4.2418" layer="42"/>
<rectangle x1="-1.27" y1="-4.2418" x2="1.27" y2="-2.3368" layer="42"/>
<rectangle x1="-2.667" y1="-4.2418" x2="-1.143" y2="-0.3048" layer="42"/>
<rectangle x1="1.143" y1="-4.2418" x2="2.667" y2="-0.3048" layer="42"/>
<rectangle x1="-1.27" y1="2.3368" x2="1.27" y2="4.2418" layer="41"/>
<rectangle x1="1.143" y1="0.3048" x2="2.667" y2="4.2418" layer="41"/>
<rectangle x1="-2.667" y1="0.3048" x2="-1.143" y2="4.2418" layer="41"/>
<rectangle x1="-1.27" y1="-4.2418" x2="1.27" y2="-2.3368" layer="41"/>
<rectangle x1="-2.667" y1="-4.2418" x2="-1.143" y2="-0.3048" layer="41"/>
<rectangle x1="1.143" y1="-4.2418" x2="2.667" y2="-0.3048" layer="41"/>
<rectangle x1="-1.27" y1="2.3368" x2="1.27" y2="4.2418" layer="43"/>
<rectangle x1="1.143" y1="0.3048" x2="2.667" y2="4.2418" layer="43"/>
<rectangle x1="-2.667" y1="0.3048" x2="-1.143" y2="4.2418" layer="43"/>
<rectangle x1="-1.27" y1="-4.2418" x2="1.27" y2="-2.3368" layer="43"/>
<rectangle x1="-2.667" y1="-4.2418" x2="-1.143" y2="-0.3048" layer="43"/>
<rectangle x1="1.143" y1="-4.2418" x2="2.667" y2="-0.3048" layer="43"/>
</package>
</packages>
<symbols>
<symbol name="KS">
<wire x1="-3.81" y1="1.905" x2="-2.54" y2="1.905" width="0.254" layer="94"/>
<wire x1="-3.81" y1="1.905" x2="-3.81" y2="0" width="0.254" layer="94"/>
<wire x1="-3.81" y1="0" x2="-1.905" y2="0" width="0.1524" layer="94"/>
<wire x1="-3.81" y1="0" x2="-3.81" y2="-1.905" width="0.254" layer="94"/>
<wire x1="-1.27" y1="0" x2="-0.762" y2="0" width="0.1524" layer="94"/>
<wire x1="-5.08" y1="1.905" x2="-3.81" y2="1.905" width="0.254" layer="94"/>
<wire x1="0.254" y1="0" x2="0.635" y2="0" width="0.1524" layer="94"/>
<wire x1="1.27" y1="0" x2="1.905" y2="0" width="0.1524" layer="94"/>
<wire x1="2.54" y1="-3.175" x2="2.54" y2="-1.905" width="0.254" layer="94"/>
<wire x1="2.54" y1="-1.905" x2="1.27" y2="1.905" width="0.254" layer="94"/>
<wire x1="2.54" y1="1.905" x2="2.54" y2="3.175" width="0.254" layer="94"/>
<wire x1="-0.762" y1="0" x2="-0.254" y2="-0.762" width="0.1524" layer="94"/>
<wire x1="-0.254" y1="-0.762" x2="0.254" y2="0" width="0.1524" layer="94"/>
<text x="-3.81" y="3.175" size="1.778" layer="95" rot="R90">&gt;NAME</text>
<text x="-1.27" y="3.175" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="P" x="2.54" y="-5.08" visible="pad" length="short" direction="pas" rot="R90"/>
<pin name="S" x="2.54" y="5.08" visible="pad" length="short" direction="pas" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="MS243" prefix="S" uservalue="yes">
<description>&lt;b&gt;TOGGLE SWITCH&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="KS" x="0" y="0"/>
</gates>
<devices>
<device name="" package="MS243">
<connects>
<connect gate="1" pin="P" pad="1"/>
<connect gate="1" pin="S" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="allegro">
<description>&lt;b&gt;Allegro MicroSystems, Inc&lt;/b&gt;&lt;p&gt;
www.allegromicro.com&lt;br&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="ACS75050">
<description>&lt;b&gt;Current Sensor&lt;/b&gt;&lt;p&gt;
Source: www.allegromicro.com/datafile/0750-050.pdf</description>
<wire x1="-7" y1="1" x2="-3" y2="1" width="0" layer="46"/>
<wire x1="-3" y1="1" x2="-3" y2="-1" width="0" layer="46" curve="-180"/>
<wire x1="-3" y1="-1" x2="-7" y2="-1" width="0" layer="46"/>
<wire x1="-7" y1="-1" x2="-7" y2="1" width="0" layer="46" curve="-180"/>
<wire x1="3" y1="1" x2="7" y2="1" width="0" layer="46"/>
<wire x1="7" y1="1" x2="7" y2="-1" width="0" layer="46" curve="-180"/>
<wire x1="7" y1="-1" x2="3" y2="-1" width="0" layer="46"/>
<wire x1="3" y1="-1" x2="3" y2="1" width="0" layer="46" curve="-180"/>
<wire x1="-7" y1="3" x2="-7" y2="-3" width="0.001" layer="49" curve="180"/>
<wire x1="-7" y1="-3" x2="-3" y2="-3" width="0.001" layer="49"/>
<wire x1="-3" y1="-3" x2="-0.5" y2="-1.65" width="0.001" layer="49" curve="56.738399"/>
<wire x1="-7" y1="3" x2="-3" y2="3" width="0.001" layer="49"/>
<wire x1="-3" y1="3" x2="-0.5" y2="1.65" width="0.001" layer="49" curve="-56.738399"/>
<wire x1="-0.5" y1="1.65" x2="-0.5" y2="-1.65" width="0.001" layer="49"/>
<wire x1="7" y1="-3" x2="7" y2="3" width="0.001" layer="49" curve="180"/>
<wire x1="7" y1="3" x2="3" y2="3" width="0.001" layer="49"/>
<wire x1="3" y1="3" x2="0.5" y2="1.65" width="0.001" layer="49" curve="56.738399"/>
<wire x1="7" y1="-3" x2="3" y2="-3" width="0.001" layer="49"/>
<wire x1="3" y1="-3" x2="0.5" y2="-1.65" width="0.001" layer="49" curve="-56.738399"/>
<wire x1="0.5" y1="1.65" x2="0.5" y2="-1.65" width="0.001" layer="49"/>
<wire x1="-7" y1="0.7" x2="-3" y2="0.7" width="0.2032" layer="51"/>
<wire x1="-3" y1="0.7" x2="-3" y2="-4.9" width="0.2032" layer="51"/>
<wire x1="-7" y1="0.7" x2="-7" y2="-4.9" width="0.2032" layer="51"/>
<wire x1="-4.95" y1="-4.9" x2="4.95" y2="-4.9" width="0.2032" layer="21"/>
<wire x1="7" y1="-4.9" x2="7" y2="0.7" width="0.2032" layer="51"/>
<wire x1="7" y1="0.7" x2="3" y2="0.7" width="0.2032" layer="51"/>
<wire x1="3" y1="0.7" x2="3" y2="-4.9" width="0.2032" layer="51"/>
<wire x1="-7" y1="-4.9" x2="-7" y2="-12.65" width="0.2032" layer="21"/>
<wire x1="-6.525" y1="-13.775" x2="-5.025" y2="-13.775" width="0.2032" layer="21"/>
<wire x1="7" y1="-4.9" x2="7" y2="-12.65" width="0.2032" layer="21"/>
<wire x1="6.525" y1="-13.775" x2="5" y2="-13.775" width="0.2032" layer="21"/>
<wire x1="-4.95" y1="-4.9" x2="-4.95" y2="-17.6" width="0.2032" layer="21"/>
<wire x1="-4.95" y1="-17.6" x2="4.95" y2="-17.6" width="0.2032" layer="21"/>
<wire x1="4.95" y1="-17.6" x2="4.95" y2="-4.9" width="0.2032" layer="21"/>
<wire x1="-6.525" y1="-13.775" x2="-7" y2="-12.65" width="0.2032" layer="21"/>
<wire x1="6.525" y1="-13.775" x2="7" y2="-12.65" width="0.2032" layer="21"/>
<wire x1="0" y1="-21.4" x2="0" y2="-20.2" width="0.6" layer="51"/>
<wire x1="0" y1="-20.2" x2="0" y2="-17.825" width="0.6" layer="21"/>
<wire x1="-1.91" y1="-21.4" x2="-1.91" y2="-20.2" width="0.6" layer="51"/>
<wire x1="-1.91" y1="-20.2" x2="-1.91" y2="-17.825" width="0.6" layer="21"/>
<wire x1="1.91" y1="-21.4" x2="1.91" y2="-20.2" width="0.6" layer="51"/>
<wire x1="1.91" y1="-20.2" x2="1.91" y2="-17.825" width="0.6" layer="21"/>
<circle x="1.8243" y="-1.6181" radius="0.25" width="0.001" layer="49"/>
<circle x="1.0978" y="-0.6181" radius="0.25" width="0.001" layer="49"/>
<circle x="1.0978" y="0.6179" radius="0.25" width="0.001" layer="49"/>
<circle x="1.8244" y="1.618" radius="0.25" width="0.001" layer="49"/>
<circle x="3" y="2" radius="0.25" width="0.001" layer="49"/>
<circle x="3" y="-2" radius="0.25" width="0.001" layer="49"/>
<circle x="4.3333" y="-2" radius="0.25" width="0.001" layer="49"/>
<circle x="5.6666" y="-2" radius="0.25" width="0.001" layer="49"/>
<circle x="8.1757" y="1.6181" radius="0.25" width="0.001" layer="49"/>
<circle x="8.9022" y="0.6181" radius="0.25" width="0.001" layer="49"/>
<circle x="8.9022" y="-0.6179" radius="0.25" width="0.001" layer="49"/>
<circle x="8.1756" y="-1.618" radius="0.25" width="0.001" layer="49"/>
<circle x="7" y="-2" radius="0.25" width="0.001" layer="49"/>
<circle x="7" y="2" radius="0.25" width="0.001" layer="49"/>
<circle x="5.6667" y="2" radius="0.25" width="0.001" layer="49"/>
<circle x="4.3334" y="2" radius="0.25" width="0.001" layer="49"/>
<circle x="-8.1757" y="-1.6181" radius="0.25" width="0.001" layer="49"/>
<circle x="-8.9022" y="-0.6181" radius="0.25" width="0.001" layer="49"/>
<circle x="-8.9022" y="0.6179" radius="0.25" width="0.001" layer="49"/>
<circle x="-8.1756" y="1.618" radius="0.25" width="0.001" layer="49"/>
<circle x="-7" y="2" radius="0.25" width="0.001" layer="49"/>
<circle x="-7" y="-2" radius="0.25" width="0.001" layer="49"/>
<circle x="-5.6667" y="-2" radius="0.25" width="0.001" layer="49"/>
<circle x="-4.3334" y="-2" radius="0.25" width="0.001" layer="49"/>
<circle x="-1.8243" y="1.6181" radius="0.25" width="0.001" layer="49"/>
<circle x="-1.0978" y="0.6181" radius="0.25" width="0.001" layer="49"/>
<circle x="-1.0978" y="-0.6179" radius="0.25" width="0.001" layer="49"/>
<circle x="-1.8244" y="-1.618" radius="0.25" width="0.001" layer="49"/>
<circle x="-3" y="-2" radius="0.25" width="0.001" layer="49"/>
<circle x="-3" y="2" radius="0.25" width="0.001" layer="49"/>
<circle x="-4.3333" y="2" radius="0.25" width="0.001" layer="49"/>
<circle x="-5.6666" y="2" radius="0.25" width="0.001" layer="49"/>
<circle x="-3.175" y="-16.225" radius="0.4776" width="0.2032" layer="21"/>
<pad name="IP+@2" x="8.89" y="-0.635" drill="0.5" diameter="0.9"/>
<pad name="IP-@2" x="-8.89" y="-0.635" drill="0.5" diameter="0.9" rot="R180"/>
<pad name="IP+" x="7" y="0" drill="2" diameter="4" shape="offset" rot="R180"/>
<pad name="IP-" x="-7" y="0" drill="2" diameter="4" shape="offset"/>
<pad name="1" x="-1.91" y="-21.4" drill="0.8" diameter="1.5"/>
<pad name="2" x="0" y="-21.4" drill="0.8" diameter="1.5"/>
<pad name="3" x="1.91" y="-21.4" drill="0.8" diameter="1.5"/>
<text x="-3.81" y="-8.255" size="1.27" layer="25">&gt;NAME</text>
<text x="-3.81" y="-13.335" size="1.27" layer="27">&gt;VALUE</text>
<pad name="IP-@3" x="-8.89" y="0.635" drill="0.5" diameter="0.9"/>
<pad name="IP+@3" x="8.89" y="0.635" drill="0.5" diameter="0.9"/>
<pad name="IP+@4" x="8.175" y="1.625" drill="0.5" diameter="0.9"/>
<pad name="IP+@5" x="7" y="2" drill="0.5" diameter="0.9"/>
<pad name="IP+@6" x="5.675" y="2" drill="0.5" diameter="0.9"/>
<pad name="IP+@7" x="4.325" y="2" drill="0.5" diameter="0.9"/>
<pad name="IP+@8" x="3" y="2" drill="0.5" diameter="0.9"/>
<pad name="IP+@9" x="1.825" y="1.625" drill="0.5" diameter="0.9"/>
<pad name="IP+@10" x="1.1" y="0.625" drill="0.5" diameter="0.9"/>
<pad name="IP+@11" x="1.1" y="-0.625" drill="0.5" diameter="0.9"/>
<pad name="IP+@12" x="1.825" y="-1.625" drill="0.5" diameter="0.9"/>
<pad name="IP+@13" x="3" y="-2" drill="0.5" diameter="0.9"/>
<pad name="IP+@14" x="4.325" y="-2" drill="0.5" diameter="0.9"/>
<pad name="IP+@15" x="5.675" y="-2" drill="0.5" diameter="0.9"/>
<pad name="IP+@16" x="7" y="-2" drill="0.5" diameter="0.9"/>
<pad name="IP+@17" x="8.175" y="-1.625" drill="0.5" diameter="0.9"/>
<pad name="IP-@4" x="-8.175" y="1.625" drill="0.5" diameter="0.9"/>
<pad name="IP-@5" x="-7" y="2" drill="0.5" diameter="0.9"/>
<pad name="IP-@6" x="-5.675" y="2" drill="0.5" diameter="0.9"/>
<pad name="IP-@7" x="-4.325" y="2" drill="0.5" diameter="0.9"/>
<pad name="IP-@8" x="-3" y="2" drill="0.5" diameter="0.9"/>
<pad name="IP-@9" x="-1.825" y="1.625" drill="0.5" diameter="0.9"/>
<pad name="IP-@10" x="-1.1" y="0.625" drill="0.5" diameter="0.9"/>
<pad name="IP-@11" x="-1.1" y="-0.625" drill="0.5" diameter="0.9"/>
<pad name="IP-@12" x="-1.825" y="-1.625" drill="0.5" diameter="0.9"/>
<pad name="IP-@13" x="-3" y="-2" drill="0.5" diameter="0.9"/>
<pad name="IP-@14" x="-4.35" y="-2" drill="0.5" diameter="0.9"/>
<pad name="IP-@15" x="-5.675" y="-2" drill="0.5" diameter="0.9"/>
<pad name="IP-@16" x="-7" y="-2" drill="0.5" diameter="0.9"/>
<pad name="IP-@17" x="-8.175" y="-1.625" drill="0.5" diameter="0.9"/>
<polygon width="0.3" layer="1">
<vertex x="-0.635" y="1.5875"/>
<vertex x="-0.635" y="-1.5875" curve="-51.371965"/>
<vertex x="-2.8575" y="-2.8575"/>
<vertex x="-6.985" y="-2.8575" curve="-180"/>
<vertex x="-6.985" y="2.8575"/>
<vertex x="-2.8575" y="2.8575" curve="-54.193064"/>
</polygon>
<polygon width="0.3" layer="1">
<vertex x="0.635" y="-1.5875"/>
<vertex x="0.635" y="1.5875" curve="-51.371965"/>
<vertex x="2.8575" y="2.8575"/>
<vertex x="6.985" y="2.8575" curve="-180"/>
<vertex x="6.985" y="-2.8575"/>
<vertex x="2.8575" y="-2.8575" curve="-54.193064"/>
</polygon>
<polygon width="0.3" layer="16">
<vertex x="-0.635" y="1.5875"/>
<vertex x="-0.635" y="-1.5875" curve="-51.371965"/>
<vertex x="-2.8575" y="-2.8575"/>
<vertex x="-6.985" y="-2.8575" curve="-180"/>
<vertex x="-6.985" y="2.8575"/>
<vertex x="-2.8575" y="2.8575" curve="-54.193064"/>
</polygon>
<polygon width="0.3" layer="16">
<vertex x="0.635" y="-1.5875"/>
<vertex x="0.635" y="1.5875" curve="-51.371965"/>
<vertex x="2.8575" y="2.8575"/>
<vertex x="6.985" y="2.8575" curve="-180"/>
<vertex x="6.985" y="-2.8575"/>
<vertex x="2.8575" y="-2.8575" curve="-54.193064"/>
</polygon>
<rectangle x1="-0.3175" y1="-2.2225" x2="0.3175" y2="2.2225" layer="41"/>
<rectangle x1="-0.3175" y1="-2.2225" x2="0.3175" y2="2.2225" layer="42"/>
<rectangle x1="-0.3175" y1="-2.2225" x2="0.3175" y2="2.2225" layer="43"/>
</package>
</packages>
<symbols>
<symbol name="ACS750XCA-050">
<wire x1="7.62" y1="7.62" x2="7.62" y2="-7.62" width="0.254" layer="94"/>
<wire x1="7.62" y1="-7.62" x2="-7.62" y2="-7.62" width="0.254" layer="94"/>
<wire x1="-7.62" y1="-7.62" x2="-7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="-7.62" y1="7.62" x2="7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="-5.08" y1="4.445" x2="-5.08" y2="1.016" width="0.254" layer="94"/>
<wire x1="-5.08" y1="0.3048" x2="-5.08" y2="-4.445" width="0.254" layer="94"/>
<wire x1="-4.572" y1="0" x2="-4.572" y2="-0.254" width="0.0508" layer="94" curve="331.927513" cap="flat"/>
<wire x1="-4.064" y1="0" x2="-4.064" y2="-0.254" width="0.0508" layer="94" curve="345.694225" cap="flat"/>
<wire x1="-4.572" y1="0" x2="-4.064" y2="0" width="0.0508" layer="94"/>
<wire x1="-4.572" y1="-0.254" x2="-4.064" y2="-0.254" width="0.0508" layer="94"/>
<text x="-2.54" y="8.89" size="1.778" layer="95">&gt;NAME</text>
<text x="-2.54" y="-10.16" size="1.778" layer="96">&gt;VALUE</text>
<text x="-6.35" y="-6.604" size="1.524" layer="95">IP-</text>
<text x="-6.35" y="5.08" size="1.524" layer="95">IP+</text>
<pin name="IP-" x="-5.08" y="-12.7" visible="pad" length="middle" direction="pas" rot="R90"/>
<pin name="IP+" x="-5.08" y="12.7" visible="pad" length="middle" direction="pas" rot="R270"/>
<pin name="GND" x="10.16" y="-5.08" length="short" direction="pwr" rot="R180"/>
<pin name="VCC" x="10.16" y="5.08" length="short" direction="pwr" rot="R180"/>
<pin name="VOUT" x="10.16" y="0" length="short" direction="out" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="ACS750*CA-050" prefix="IC">
<description>&lt;b&gt;Current Sensor&lt;/b&gt;&lt;p&gt;
display layer 49 Reference in BRD and place VIAs on circles&lt;br&gt;
Source: www.allegromicro.com/datafile/0750-050.pdf</description>
<gates>
<gate name="G$1" symbol="ACS750XCA-050" x="0" y="0"/>
</gates>
<devices>
<device name="" package="ACS75050">
<connects>
<connect gate="G$1" pin="GND" pad="2"/>
<connect gate="G$1" pin="IP+" pad="IP+ IP+@2 IP+@3 IP+@4 IP+@5 IP+@6 IP+@7 IP+@8 IP+@9 IP+@10 IP+@11 IP+@12 IP+@13 IP+@14 IP+@15 IP+@16 IP+@17" route="any"/>
<connect gate="G$1" pin="IP-" pad="IP- IP-@2 IP-@3 IP-@4 IP-@5 IP-@6 IP-@7 IP-@8 IP-@9 IP-@10 IP-@11 IP-@12 IP-@13 IP-@14 IP-@15 IP-@16 IP-@17" route="any"/>
<connect gate="G$1" pin="VCC" pad="1"/>
<connect gate="G$1" pin="VOUT" pad="3"/>
</connects>
<technologies>
<technology name="L">
<attribute name="MF" value="ALLEGRO MICROSYSTEMS, INC." constant="no"/>
<attribute name="MPN" value="ACS750LCA-050" constant="no"/>
<attribute name="OC_FARNELL" value="1457176" constant="no"/>
<attribute name="OC_NEWARK" value="61H4464" constant="no"/>
</technology>
<technology name="S">
<attribute name="MF" value="ALLEGRO MICROSYSTEMS, INC." constant="no"/>
<attribute name="MPN" value="ACS750SCA-050" constant="no"/>
<attribute name="OC_FARNELL" value="1457175" constant="no"/>
<attribute name="OC_NEWARK" value="81H6588" constant="no"/>
</technology>
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
<part name="J1" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J2" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J3" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J4" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="G1" library="battery" deviceset="SL-150-1/2AA/PR" device="" value="12V"/>
<part name="G2" library="battery" deviceset="SL-150-1/2AA/PR" device="" value="12V"/>
<part name="G3" library="battery" deviceset="SL-150-1/2AA/PR" device="" value="12V"/>
<part name="J5" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J6" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J7" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J8" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="G4" library="battery" deviceset="SL-150-1/2AA/PR" device="" value="12V"/>
<part name="J10" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J11" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J12" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J9" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J13" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J14" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J15" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J16" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J17" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J18" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J19" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J20" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J21" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J22" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J23" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J24" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J25" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J26" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J27" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J28" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J29" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J30" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J31" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="F1" library="special" deviceset="FUSE" device="" value="20 A"/>
<part name="F2" library="special" deviceset="FUSE" device="" value="12 A"/>
<part name="F3" library="special" deviceset="FUSE" device="" value="5 A"/>
<part name="F4" library="special" deviceset="FUSE" device="" value="5 A"/>
<part name="S3" library="switch" deviceset="MS243" device=""/>
<part name="IC1" library="allegro" deviceset="ACS750*CA-050" device="" technology="L"/>
<part name="IC2" library="allegro" deviceset="ACS750*CA-050" device="" technology="L"/>
<part name="J32" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J33" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J34" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J35" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J36" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="S1" library="switch" deviceset="MS243" device=""/>
<part name="J37" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J38" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J39" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J40" library="con-faston" deviceset="PIN1-2" device=""/>
<part name="J41" library="con-faston" deviceset="PIN1-2" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<text x="-8.89" y="74.93" size="1.778" layer="91">Red Anderson</text>
<text x="-8.89" y="66.04" size="1.778" layer="91">Black Anderson</text>
<text x="177.8" y="44.45" size="1.778" layer="91">MOTOR CONTROLLER</text>
<text x="-25.4" y="81.28" size="1.778" layer="91">BATTERY</text>
<text x="-8.89" y="57.15" size="1.778" layer="91">Red Anderson</text>
<text x="-8.89" y="48.26" size="1.778" layer="91">Black Anderson</text>
<text x="-8.89" y="40.64" size="1.778" layer="91">Red Anderson</text>
<text x="-8.89" y="31.75" size="1.778" layer="91">Black Anderson</text>
<text x="-8.89" y="24.13" size="1.778" layer="91">Red Anderson</text>
<text x="-8.89" y="15.24" size="1.778" layer="91">Black Anderson</text>
<text x="66.04" y="31.75" size="1.778" layer="91">Red</text>
<text x="24.13" y="73.66" size="1.778" layer="91">Red</text>
<text x="24.13" y="66.04" size="1.778" layer="91">Purple</text>
<text x="24.13" y="55.88" size="1.778" layer="91">Purple</text>
<text x="24.13" y="48.26" size="1.778" layer="91">Orange</text>
<text x="67.31" y="15.24" size="1.778" layer="91">Orange</text>
<text x="22.86" y="22.86" size="1.778" layer="91">Orange</text>
<text x="24.13" y="38.1" size="1.778" layer="91">Orange</text>
<text x="22.86" y="31.75" size="1.778" layer="91">Black</text>
<text x="24.13" y="15.24" size="1.778" layer="91">Black</text>
<text x="68.58" y="21.59" size="1.778" layer="91">Black</text>
<text x="87.63" y="30.48" size="1.778" layer="91">Red</text>
<text x="86.36" y="15.24" size="1.778" layer="91">Orange</text>
<text x="86.36" y="22.86" size="1.778" layer="91">Black</text>
<text x="100.33" y="-15.24" size="1.778" layer="91">Panel</text>
<text x="231.14" y="15.24" size="1.778" layer="91">Steer servo</text>
<text x="231.14" y="-10.16" size="1.778" layer="91">Brake servo</text>
<text x="109.22" y="-2.54" size="1.778" layer="91">Orange</text>
<text x="121.92" y="-2.54" size="1.778" layer="91">Orange</text>
<text x="109.22" y="-10.16" size="1.778" layer="91">Orange</text>
<text x="121.92" y="-10.16" size="1.778" layer="91">Orange</text>
<text x="139.7" y="13.97" size="1.778" layer="91">Orange</text>
<text x="157.48" y="13.97" size="1.778" layer="91">Orange</text>
<text x="96.52" y="-2.54" size="1.778" layer="91">Black</text>
<text x="139.7" y="21.59" size="1.778" layer="91">Black</text>
<text x="160.02" y="22.86" size="1.778" layer="91">Black</text>
<text x="106.68" y="45.72" size="1.778" layer="91" rot="R180">Black</text>
<text x="115.57" y="55.88" size="1.778" layer="91">Red</text>
<text x="127" y="55.88" size="1.778" layer="91">Red</text>
<text x="115.57" y="49.53" size="1.778" layer="91">Red</text>
<text x="124.46" y="49.53" size="1.778" layer="91">Red</text>
<text x="166.37" y="41.91" size="1.778" layer="91">Red</text>
<text x="180.34" y="41.91" size="1.778" layer="91">Red</text>
<text x="166.37" y="34.29" size="1.778" layer="91">Black</text>
<text x="184.15" y="34.29" size="1.778" layer="91">Black</text>
<text x="132.08" y="49.53" size="1.778" layer="91">Orange</text>
<text x="102.87" y="57.15" size="1.778" layer="91">JOYSTICK</text>
<text x="73.66" y="34.29" size="1.778" layer="91">AWG 10 on all wires</text>
</plain>
<instances>
<instance part="J1" gate="G$1" x="90.17" y="27.94" rot="R180"/>
<instance part="J2" gate="G$1" x="90.17" y="12.7" rot="R180"/>
<instance part="J3" gate="G$1" x="121.92" y="-3.81" rot="R270"/>
<instance part="J4" gate="G$1" x="111.76" y="-12.7" rot="R90"/>
<instance part="G1" gate="G$1" x="-20.32" y="71.12"/>
<instance part="G2" gate="G$1" x="-20.32" y="53.34"/>
<instance part="G3" gate="G$1" x="-20.32" y="36.83"/>
<instance part="J5" gate="G$1" x="-2.54" y="71.12"/>
<instance part="J6" gate="G$1" x="-2.54" y="63.5"/>
<instance part="J7" gate="G$1" x="104.14" y="-3.81" rot="R270"/>
<instance part="J8" gate="G$1" x="111.76" y="-3.81" rot="R270"/>
<instance part="G4" gate="G$1" x="-20.32" y="20.32"/>
<instance part="J10" gate="G$1" x="90.17" y="20.32" rot="R180"/>
<instance part="J11" gate="G$1" x="-2.54" y="53.34"/>
<instance part="J12" gate="G$1" x="-2.54" y="45.72"/>
<instance part="J9" gate="G$1" x="-2.54" y="36.83"/>
<instance part="J13" gate="G$1" x="-2.54" y="29.21"/>
<instance part="J14" gate="G$1" x="-2.54" y="20.32"/>
<instance part="J15" gate="G$1" x="-2.54" y="12.7"/>
<instance part="J16" gate="G$1" x="26.67" y="12.7" rot="R180"/>
<instance part="J17" gate="G$1" x="26.67" y="20.32" rot="R180"/>
<instance part="J18" gate="G$1" x="26.67" y="29.21" rot="R180"/>
<instance part="J19" gate="G$1" x="26.67" y="36.83" rot="R180"/>
<instance part="J20" gate="G$1" x="26.67" y="45.72" rot="R180"/>
<instance part="J21" gate="G$1" x="26.67" y="53.34" rot="R180"/>
<instance part="J22" gate="G$1" x="26.67" y="63.5" rot="R180"/>
<instance part="J23" gate="G$1" x="26.67" y="71.12" rot="R180"/>
<instance part="J24" gate="G$1" x="69.85" y="20.32"/>
<instance part="J25" gate="G$1" x="69.85" y="27.94"/>
<instance part="J26" gate="G$1" x="69.85" y="12.7"/>
<instance part="J27" gate="G$1" x="161.29" y="12.7" rot="R180"/>
<instance part="J28" gate="G$1" x="121.92" y="-12.7" rot="R90"/>
<instance part="J29" gate="G$1" x="146.05" y="20.32"/>
<instance part="J30" gate="G$1" x="146.05" y="12.7"/>
<instance part="J31" gate="G$1" x="161.29" y="20.32" rot="R180"/>
<instance part="F1" gate="G$1" x="52.07" y="71.12"/>
<instance part="F2" gate="G$1" x="55.88" y="12.7"/>
<instance part="F3" gate="G$1" x="187.96" y="12.7"/>
<instance part="F4" gate="G$1" x="187.96" y="-12.7"/>
<instance part="S3" gate="1" x="116.84" y="-20.32" rot="R90"/>
<instance part="IC1" gate="G$1" x="218.44" y="7.62" rot="R270"/>
<instance part="IC2" gate="G$1" x="218.44" y="-17.78" rot="R270"/>
<instance part="J32" gate="G$1" x="115.57" y="46.99" rot="R90"/>
<instance part="J33" gate="G$1" x="125.73" y="55.88" rot="R270"/>
<instance part="J34" gate="G$1" x="107.95" y="46.99" rot="R90"/>
<instance part="J35" gate="G$1" x="125.73" y="46.99" rot="R90"/>
<instance part="J36" gate="G$1" x="115.57" y="55.88" rot="R270"/>
<instance part="S1" gate="1" x="120.65" y="63.5" rot="R270"/>
<instance part="J37" gate="G$1" x="133.35" y="46.99" rot="R90"/>
<instance part="J38" gate="G$1" x="171.45" y="39.37"/>
<instance part="J39" gate="G$1" x="171.45" y="34.29"/>
<instance part="J40" gate="G$1" x="184.15" y="39.37" rot="R180"/>
<instance part="J41" gate="G$1" x="184.15" y="34.29" rot="R180"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<wire x1="21.59" y1="-41.91" x2="19.05" y2="-41.91" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="J5" gate="G$1" pin="1"/>
<wire x1="-16.51" y1="71.12" x2="-7.62" y2="71.12" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="G1" gate="G$1" pin="-"/>
<wire x1="-25.4" y1="71.12" x2="-25.4" y2="63.5" width="0.1524" layer="91"/>
<pinref part="J6" gate="G$1" pin="1"/>
<wire x1="-25.4" y1="63.5" x2="-7.62" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="J11" gate="G$1" pin="1"/>
<pinref part="G2" gate="G$1" pin="+"/>
<wire x1="-7.62" y1="53.34" x2="-15.24" y2="53.34" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="J12" gate="G$1" pin="1"/>
<wire x1="-7.62" y1="45.72" x2="-25.4" y2="45.72" width="0.1524" layer="91"/>
<pinref part="G2" gate="G$1" pin="-"/>
<wire x1="-25.4" y1="45.72" x2="-25.4" y2="53.34" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="J9" gate="G$1" pin="1"/>
<pinref part="G3" gate="G$1" pin="+"/>
<wire x1="-7.62" y1="36.83" x2="-15.24" y2="36.83" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="J13" gate="G$1" pin="1"/>
<pinref part="G3" gate="G$1" pin="-"/>
<wire x1="-7.62" y1="29.21" x2="-25.4" y2="29.21" width="0.1524" layer="91"/>
<wire x1="-25.4" y1="29.21" x2="-25.4" y2="36.83" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="J14" gate="G$1" pin="1"/>
<pinref part="G4" gate="G$1" pin="+"/>
<wire x1="-7.62" y1="20.32" x2="-15.24" y2="20.32" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<pinref part="J15" gate="G$1" pin="1"/>
<wire x1="-7.62" y1="12.7" x2="-25.4" y2="12.7" width="0.1524" layer="91"/>
<wire x1="-25.4" y1="12.7" x2="-25.4" y2="20.32" width="0.1524" layer="91"/>
<pinref part="G4" gate="G$1" pin="-"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<pinref part="J16" gate="G$1" pin="1"/>
<wire x1="31.75" y1="12.7" x2="36.83" y2="12.7" width="0.1524" layer="91"/>
<pinref part="J18" gate="G$1" pin="1"/>
<wire x1="31.75" y1="29.21" x2="36.83" y2="29.21" width="0.1524" layer="91"/>
<wire x1="36.83" y1="29.21" x2="36.83" y2="12.7" width="0.1524" layer="91"/>
<wire x1="36.83" y1="29.21" x2="54.61" y2="29.21" width="0.1524" layer="91"/>
<wire x1="54.61" y1="29.21" x2="54.61" y2="20.32" width="0.1524" layer="91"/>
<pinref part="J24" gate="G$1" pin="1"/>
<wire x1="54.61" y1="20.32" x2="64.77" y2="20.32" width="0.1524" layer="91"/>
<junction x="36.83" y="29.21"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<pinref part="J23" gate="G$1" pin="1"/>
<pinref part="F1" gate="G$1" pin="1"/>
<wire x1="31.75" y1="71.12" x2="44.45" y2="71.12" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$19" class="0">
<segment>
<pinref part="F1" gate="G$1" pin="2"/>
<pinref part="J25" gate="G$1" pin="1"/>
<wire x1="57.15" y1="71.12" x2="63.5" y2="71.12" width="0.1524" layer="91"/>
<wire x1="63.5" y1="71.12" x2="63.5" y2="27.94" width="0.1524" layer="91"/>
<wire x1="63.5" y1="27.94" x2="64.77" y2="27.94" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$21" class="0">
<segment>
<pinref part="F2" gate="G$1" pin="2"/>
<pinref part="J26" gate="G$1" pin="1"/>
<wire x1="60.96" y1="12.7" x2="64.77" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$26" class="0">
<segment>
<pinref part="J19" gate="G$1" pin="1"/>
<wire x1="31.75" y1="36.83" x2="40.64" y2="36.83" width="0.1524" layer="91"/>
<wire x1="40.64" y1="36.83" x2="40.64" y2="45.72" width="0.1524" layer="91"/>
<pinref part="J20" gate="G$1" pin="1"/>
<wire x1="40.64" y1="45.72" x2="31.75" y2="45.72" width="0.1524" layer="91"/>
<pinref part="J17" gate="G$1" pin="1"/>
<wire x1="31.75" y1="20.32" x2="40.64" y2="20.32" width="0.1524" layer="91"/>
<wire x1="40.64" y1="36.83" x2="40.64" y2="20.32" width="0.1524" layer="91"/>
<junction x="40.64" y="36.83"/>
<wire x1="40.64" y1="20.32" x2="40.64" y2="12.7" width="0.1524" layer="91"/>
<wire x1="40.64" y1="12.7" x2="48.26" y2="12.7" width="0.1524" layer="91"/>
<junction x="40.64" y="20.32"/>
</segment>
</net>
<net name="N$27" class="0">
<segment>
<pinref part="J21" gate="G$1" pin="1"/>
<wire x1="31.75" y1="53.34" x2="36.83" y2="53.34" width="0.1524" layer="91"/>
<wire x1="36.83" y1="53.34" x2="36.83" y2="63.5" width="0.1524" layer="91"/>
<pinref part="J22" gate="G$1" pin="1"/>
<wire x1="36.83" y1="63.5" x2="31.75" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="J10" gate="G$1" pin="1"/>
<wire x1="95.25" y1="20.32" x2="104.14" y2="20.32" width="0.1524" layer="91"/>
<pinref part="J7" gate="G$1" pin="1"/>
<wire x1="104.14" y1="20.32" x2="104.14" y2="1.27" width="0.1524" layer="91"/>
<pinref part="J29" gate="G$1" pin="1"/>
<wire x1="104.14" y1="20.32" x2="107.95" y2="20.32" width="0.1524" layer="91"/>
<junction x="104.14" y="20.32"/>
<pinref part="J34" gate="G$1" pin="1"/>
<wire x1="107.95" y1="20.32" x2="137.16" y2="20.32" width="0.1524" layer="91"/>
<wire x1="137.16" y1="20.32" x2="140.97" y2="20.32" width="0.1524" layer="91"/>
<wire x1="107.95" y1="41.91" x2="107.95" y2="20.32" width="0.1524" layer="91"/>
<junction x="107.95" y="20.32"/>
<junction x="137.16" y="20.32"/>
<wire x1="137.16" y1="20.32" x2="137.16" y2="34.29" width="0.1524" layer="91"/>
<wire x1="137.16" y1="34.29" x2="166.37" y2="34.29" width="0.1524" layer="91"/>
<pinref part="J39" gate="G$1" pin="1"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="J2" gate="G$1" pin="1"/>
<wire x1="95.25" y1="12.7" x2="111.76" y2="12.7" width="0.1524" layer="91"/>
<pinref part="J8" gate="G$1" pin="1"/>
<wire x1="111.76" y1="12.7" x2="111.76" y2="1.27" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="J4" gate="G$1" pin="1"/>
<pinref part="S3" gate="1" pin="S"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="J28" gate="G$1" pin="1"/>
<pinref part="S3" gate="1" pin="P"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="J3" gate="G$1" pin="1"/>
<wire x1="121.92" y1="1.27" x2="121.92" y2="12.7" width="0.1524" layer="91"/>
<pinref part="J30" gate="G$1" pin="1"/>
<wire x1="121.92" y1="12.7" x2="133.35" y2="12.7" width="0.1524" layer="91"/>
<pinref part="J37" gate="G$1" pin="1"/>
<wire x1="133.35" y1="12.7" x2="140.97" y2="12.7" width="0.1524" layer="91"/>
<wire x1="133.35" y1="41.91" x2="133.35" y2="12.7" width="0.1524" layer="91"/>
<junction x="133.35" y="12.7"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<pinref part="J27" gate="G$1" pin="1"/>
<pinref part="F3" gate="G$1" pin="1"/>
<wire x1="166.37" y1="12.7" x2="172.72" y2="12.7" width="0.1524" layer="91"/>
<wire x1="172.72" y1="12.7" x2="180.34" y2="12.7" width="0.1524" layer="91"/>
<wire x1="172.72" y1="12.7" x2="172.72" y2="-12.7" width="0.1524" layer="91"/>
<pinref part="F4" gate="G$1" pin="1"/>
<wire x1="172.72" y1="-12.7" x2="180.34" y2="-12.7" width="0.1524" layer="91"/>
<junction x="172.72" y="12.7"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<pinref part="F3" gate="G$1" pin="2"/>
<wire x1="193.04" y1="12.7" x2="205.74" y2="12.7" width="0.1524" layer="91"/>
<pinref part="IC1" gate="G$1" pin="IP-"/>
<wire x1="205.74" y1="12.7" x2="207.01" y2="12.7" width="0.1524" layer="91"/>
<junction x="205.74" y="12.7"/>
</segment>
</net>
<net name="N$20" class="0">
<segment>
<pinref part="F4" gate="G$1" pin="2"/>
<wire x1="193.04" y1="-12.7" x2="205.74" y2="-12.7" width="0.1524" layer="91"/>
<pinref part="IC2" gate="G$1" pin="IP-"/>
<wire x1="205.74" y1="-12.7" x2="207.01" y2="-12.7" width="0.1524" layer="91"/>
<junction x="205.74" y="-12.7"/>
</segment>
</net>
<net name="N$22" class="0">
<segment>
<wire x1="165.1" y1="20.32" x2="177.8" y2="20.32" width="0.1524" layer="91"/>
<wire x1="177.8" y1="20.32" x2="233.68" y2="20.32" width="0.1524" layer="91"/>
<wire x1="177.8" y1="20.32" x2="177.8" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="177.8" y1="-5.08" x2="233.68" y2="-5.08" width="0.1524" layer="91"/>
<junction x="177.8" y="20.32"/>
</segment>
</net>
<net name="N$23" class="0">
<segment>
<pinref part="IC2" gate="G$1" pin="IP+"/>
<wire x1="231.14" y1="-12.7" x2="236.22" y2="-12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$24" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="IP+"/>
<wire x1="231.14" y1="12.7" x2="236.22" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$29" class="0">
<segment>
<pinref part="J33" gate="G$1" pin="1"/>
<pinref part="S1" gate="1" pin="S"/>
</segment>
</net>
<net name="N$30" class="0">
<segment>
<pinref part="J36" gate="G$1" pin="1"/>
<pinref part="S1" gate="1" pin="P"/>
</segment>
</net>
<net name="N$25" class="0">
<segment>
<pinref part="J1" gate="G$1" pin="1"/>
<wire x1="95.25" y1="27.94" x2="115.57" y2="27.94" width="0.1524" layer="91"/>
<pinref part="J32" gate="G$1" pin="1"/>
<wire x1="115.57" y1="27.94" x2="115.57" y2="41.91" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$28" class="0">
<segment>
<pinref part="J35" gate="G$1" pin="1"/>
<wire x1="125.73" y1="41.91" x2="125.73" y2="39.37" width="0.1524" layer="91"/>
<wire x1="125.73" y1="39.37" x2="166.37" y2="39.37" width="0.1524" layer="91"/>
<pinref part="J38" gate="G$1" pin="1"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
