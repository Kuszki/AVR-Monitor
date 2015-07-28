<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.2.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="4" name="Route4" color="1" fill="4" visible="no" active="no"/>
<layer number="5" name="Route5" color="4" fill="4" visible="no" active="no"/>
<layer number="6" name="Route6" color="1" fill="8" visible="no" active="no"/>
<layer number="7" name="Route7" color="4" fill="8" visible="no" active="no"/>
<layer number="8" name="Route8" color="1" fill="2" visible="no" active="no"/>
<layer number="9" name="Route9" color="4" fill="2" visible="no" active="no"/>
<layer number="10" name="Route10" color="1" fill="7" visible="no" active="no"/>
<layer number="11" name="Route11" color="4" fill="7" visible="no" active="no"/>
<layer number="12" name="Route12" color="1" fill="5" visible="no" active="no"/>
<layer number="13" name="Route13" color="4" fill="5" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
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
<library name="kostka">
<packages>
<package name="X8">
<hole x="0" y="0" drill="2.5"/>
<hole x="0" y="8" drill="2.5"/>
<hole x="0" y="16" drill="2.5"/>
<hole x="0" y="-8" drill="2.5"/>
<hole x="0" y="-16" drill="2.5"/>
<hole x="0" y="-24" drill="2.5"/>
<hole x="0" y="24" drill="2.5"/>
<wire x1="-7.75" y1="30.5" x2="7.75" y2="30.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="30.5" x2="7.75" y2="25.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="25.5" x2="-7.75" y2="25.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="25.5" x2="-7.75" y2="30.5" width="0.127" layer="21"/>
<pad name="P$1" x="-9.23" y="28" drill="1" diameter="2.54"/>
<pad name="P$2" x="-9.23" y="20" drill="1" diameter="2.54"/>
<pad name="P$3" x="-9.23" y="12" drill="1" diameter="2.54"/>
<pad name="P$4" x="-9.23" y="4" drill="1" diameter="2.54"/>
<pad name="P$5" x="-9.23" y="-4" drill="1" diameter="2.54"/>
<pad name="P$6" x="-9.23" y="-12" drill="1" diameter="2.54"/>
<pad name="P$7" x="-9.23" y="-20" drill="1" diameter="2.54"/>
<pad name="P$8" x="-9.23" y="-28" drill="1" diameter="2.54"/>
<wire x1="-7.75" y1="22.5" x2="7.75" y2="22.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="22.5" x2="7.75" y2="17.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="17.5" x2="-7.75" y2="17.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="17.5" x2="-7.75" y2="22.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="14.5" x2="7.75" y2="14.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="14.5" x2="7.75" y2="9.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="9.5" x2="-7.75" y2="9.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="9.5" x2="-7.75" y2="14.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="6.5" x2="7.75" y2="6.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="6.5" x2="7.75" y2="1.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="1.5" x2="-7.75" y2="1.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="1.5" x2="-7.75" y2="6.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-1.5" x2="7.75" y2="-1.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-1.5" x2="7.75" y2="-6.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-6.5" x2="-7.75" y2="-6.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-6.5" x2="-7.75" y2="-1.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-9.5" x2="7.75" y2="-9.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-9.5" x2="7.75" y2="-14.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-14.5" x2="-7.75" y2="-14.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-14.5" x2="-7.75" y2="-9.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-17.5" x2="7.75" y2="-17.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-17.5" x2="7.75" y2="-22.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-22.5" x2="-7.75" y2="-22.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-22.5" x2="-7.75" y2="-17.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-25.5" x2="7.75" y2="-25.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-25.5" x2="7.75" y2="-30.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-30.5" x2="-7.75" y2="-30.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-30.5" x2="-7.75" y2="-25.5" width="0.127" layer="21"/>
</package>
<package name="X6">
<hole x="0" y="-16" drill="2.5"/>
<hole x="0" y="16" drill="2.5"/>
<hole x="0" y="8" drill="2.5"/>
<hole x="0" y="0" drill="2.5"/>
<hole x="0" y="-8" drill="2.5"/>
<wire x1="-7.75" y1="22.5" x2="7.75" y2="22.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="22.5" x2="7.75" y2="17.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="17.5" x2="-7.75" y2="17.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="17.5" x2="-7.75" y2="22.5" width="0.127" layer="21"/>
<pad name="P$1" x="-9.23" y="20" drill="1" diameter="2.54"/>
<pad name="P$2" x="-9.23" y="12" drill="1" diameter="2.54"/>
<pad name="P$3" x="-9.23" y="4" drill="1" diameter="2.54"/>
<pad name="P$4" x="-9.23" y="-4" drill="1" diameter="2.54"/>
<pad name="P$5" x="-9.23" y="-12" drill="1" diameter="2.54"/>
<pad name="P$6" x="-9.23" y="-20" drill="1" diameter="2.54"/>
<wire x1="-7.75" y1="14.5" x2="7.75" y2="14.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="14.5" x2="7.75" y2="9.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="9.5" x2="-7.75" y2="9.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="9.5" x2="-7.75" y2="14.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="6.5" x2="7.75" y2="6.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="6.5" x2="7.75" y2="1.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="1.5" x2="-7.75" y2="1.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="1.5" x2="-7.75" y2="6.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-1.5" x2="7.75" y2="-1.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-1.5" x2="7.75" y2="-6.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-6.5" x2="-7.75" y2="-6.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-6.5" x2="-7.75" y2="-1.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-9.5" x2="7.75" y2="-9.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-9.5" x2="7.75" y2="-14.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-14.5" x2="-7.75" y2="-14.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-14.5" x2="-7.75" y2="-9.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-17.5" x2="7.75" y2="-17.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-17.5" x2="7.75" y2="-22.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-22.5" x2="-7.75" y2="-22.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-22.5" x2="-7.75" y2="-17.5" width="0.127" layer="21"/>
</package>
<package name="X5">
<hole x="0" y="4" drill="2.5"/>
<hole x="0" y="-4" drill="2.5"/>
<hole x="0" y="12" drill="2.5"/>
<hole x="0" y="-12" drill="2.5"/>
<wire x1="-7.75" y1="18.5" x2="7.75" y2="18.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="18.5" x2="7.75" y2="13.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="13.5" x2="-7.75" y2="13.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="13.5" x2="-7.75" y2="18.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="10.5" x2="7.75" y2="10.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="10.5" x2="7.75" y2="5.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="5.5" x2="-7.75" y2="5.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="5.5" x2="-7.75" y2="10.5" width="0.127" layer="21"/>
<pad name="P$1" x="-9.23" y="16" drill="1" diameter="2.54"/>
<pad name="P$2" x="-9.23" y="8" drill="1" diameter="2.54"/>
<pad name="P$3" x="-9.23" y="0" drill="1" diameter="2.54"/>
<pad name="P$4" x="-9.23" y="-8" drill="1" diameter="2.54"/>
<pad name="P$5" x="-9.23" y="-16" drill="1" diameter="2.54"/>
<wire x1="-7.74" y1="2.38" x2="7.76" y2="2.38" width="0.127" layer="21"/>
<wire x1="7.76" y1="2.38" x2="7.76" y2="-2.62" width="0.127" layer="21"/>
<wire x1="7.76" y1="-2.62" x2="-7.74" y2="-2.62" width="0.127" layer="21"/>
<wire x1="-7.74" y1="-2.62" x2="-7.74" y2="2.38" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-5.5" x2="7.75" y2="-5.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-5.5" x2="7.75" y2="-10.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-10.5" x2="-7.75" y2="-10.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-10.5" x2="-7.75" y2="-5.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-13.5" x2="7.75" y2="-13.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-13.5" x2="7.75" y2="-18.5" width="0.127" layer="21"/>
<wire x1="7.75" y1="-18.5" x2="-7.75" y2="-18.5" width="0.127" layer="21"/>
<wire x1="-7.75" y1="-18.5" x2="-7.75" y2="-13.5" width="0.127" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="X8">
<pin name="P$1" x="-8.89" y="8.89" length="middle"/>
<pin name="P$2" x="-8.89" y="6.35" length="middle"/>
<pin name="P$3" x="-8.89" y="3.81" length="middle"/>
<pin name="P$4" x="-8.89" y="1.27" length="middle"/>
<pin name="P$5" x="-8.89" y="-1.27" length="middle"/>
<pin name="P$6" x="-8.89" y="-3.81" length="middle"/>
<pin name="P$7" x="-8.89" y="-6.35" length="middle"/>
<pin name="P$8" x="-8.89" y="-8.89" length="middle"/>
<wire x1="-3.81" y1="11.43" x2="3.81" y2="11.43" width="0.254" layer="94"/>
<wire x1="3.81" y1="11.43" x2="3.81" y2="-11.43" width="0.254" layer="94"/>
<wire x1="3.81" y1="-11.43" x2="-3.81" y2="-11.43" width="0.254" layer="94"/>
<wire x1="-3.81" y1="-11.43" x2="-3.81" y2="11.43" width="0.254" layer="94"/>
</symbol>
<symbol name="X6">
<pin name="P$1" x="-5.08" y="0" length="middle"/>
<pin name="P$2" x="-5.08" y="-2.54" length="middle"/>
<pin name="P$3" x="-5.08" y="-5.08" length="middle"/>
<pin name="P$4" x="-5.08" y="-7.62" length="middle"/>
<pin name="P$5" x="-5.08" y="-10.16" length="middle"/>
<pin name="P$6" x="-5.08" y="-12.7" length="middle"/>
<wire x1="0" y1="2.54" x2="7.62" y2="2.54" width="0.254" layer="94"/>
<wire x1="7.62" y1="2.54" x2="7.62" y2="-15.24" width="0.254" layer="94"/>
<wire x1="7.62" y1="-15.24" x2="0" y2="-15.24" width="0.254" layer="94"/>
<wire x1="0" y1="-15.24" x2="0" y2="2.54" width="0.254" layer="94"/>
</symbol>
<symbol name="X5">
<pin name="P$1" x="-8.89" y="5.08" length="middle"/>
<pin name="P$2" x="-8.89" y="2.54" length="middle"/>
<pin name="P$3" x="-8.89" y="0" length="middle"/>
<pin name="P$4" x="-8.89" y="-2.54" length="middle"/>
<pin name="P$5" x="-8.89" y="-5.08" length="middle"/>
<wire x1="-3.81" y1="7.62" x2="3.81" y2="7.62" width="0.254" layer="94"/>
<wire x1="3.81" y1="7.62" x2="3.81" y2="-7.62" width="0.254" layer="94"/>
<wire x1="3.81" y1="-7.62" x2="-3.81" y2="-7.62" width="0.254" layer="94"/>
<wire x1="-3.81" y1="-7.62" x2="-3.81" y2="7.62" width="0.254" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="KOSTKA_2.5MM_X8">
<gates>
<gate name="G$1" symbol="X8" x="0" y="0"/>
</gates>
<devices>
<device name="" package="X8">
<connects>
<connect gate="G$1" pin="P$1" pad="P$1"/>
<connect gate="G$1" pin="P$2" pad="P$2"/>
<connect gate="G$1" pin="P$3" pad="P$3"/>
<connect gate="G$1" pin="P$4" pad="P$4"/>
<connect gate="G$1" pin="P$5" pad="P$5"/>
<connect gate="G$1" pin="P$6" pad="P$6"/>
<connect gate="G$1" pin="P$7" pad="P$7"/>
<connect gate="G$1" pin="P$8" pad="P$8"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="KOSTKA_2.5MM_X6">
<gates>
<gate name="G$1" symbol="X6" x="-3.81" y="6.096"/>
</gates>
<devices>
<device name="" package="X6">
<connects>
<connect gate="G$1" pin="P$1" pad="P$1"/>
<connect gate="G$1" pin="P$2" pad="P$2"/>
<connect gate="G$1" pin="P$3" pad="P$3"/>
<connect gate="G$1" pin="P$4" pad="P$4"/>
<connect gate="G$1" pin="P$5" pad="P$5"/>
<connect gate="G$1" pin="P$6" pad="P$6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="KOSTKA_2.5MM_X5">
<gates>
<gate name="G$1" symbol="X5" x="0" y="0"/>
</gates>
<devices>
<device name="" package="X5">
<connects>
<connect gate="G$1" pin="P$1" pad="P$1"/>
<connect gate="G$1" pin="P$2" pad="P$2"/>
<connect gate="G$1" pin="P$3" pad="P$3"/>
<connect gate="G$1" pin="P$4" pad="P$4"/>
<connect gate="G$1" pin="P$5" pad="P$5"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="con-lstb">
<description>&lt;b&gt;Pin Headers&lt;/b&gt;&lt;p&gt;
Naming:&lt;p&gt;
MA = male&lt;p&gt;
# contacts - # rows&lt;p&gt;
W = angled&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="MA08-1">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-9.525" y1="1.27" x2="-8.255" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-8.255" y1="1.27" x2="-7.62" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="-0.635" x2="-8.255" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="0.635" x2="-6.985" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="1.27" x2="-5.715" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="1.27" x2="-5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-0.635" x2="-5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="-1.27" x2="-6.985" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="-1.27" x2="-7.62" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-10.16" y1="0.635" x2="-10.16" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-9.525" y1="1.27" x2="-10.16" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-10.16" y1="-0.635" x2="-9.525" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-8.255" y1="-1.27" x2="-9.525" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="0.635" x2="-4.445" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="1.27" x2="-3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="1.27" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-0.635" x2="-3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-1.27" x2="-4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="-1.27" x2="-5.08" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="1.27" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.635" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-0.635" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.27" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-0.635" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="1.27" x2="4.445" y2="1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="1.27" x2="5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-0.635" x2="4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="-1.27" x2="3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-1.27" x2="2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.635" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.08" y1="0.635" x2="5.715" y2="1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="1.27" x2="6.985" y2="1.27" width="0.1524" layer="21"/>
<wire x1="6.985" y1="1.27" x2="7.62" y2="0.635" width="0.1524" layer="21"/>
<wire x1="7.62" y1="-0.635" x2="6.985" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="6.985" y1="-1.27" x2="5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="-1.27" x2="5.08" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="8.255" y1="1.27" x2="9.525" y2="1.27" width="0.1524" layer="21"/>
<wire x1="9.525" y1="1.27" x2="10.16" y2="0.635" width="0.1524" layer="21"/>
<wire x1="10.16" y1="0.635" x2="10.16" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="10.16" y1="-0.635" x2="9.525" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="8.255" y1="1.27" x2="7.62" y2="0.635" width="0.1524" layer="21"/>
<wire x1="7.62" y1="-0.635" x2="8.255" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="9.525" y1="-1.27" x2="8.255" y2="-1.27" width="0.1524" layer="21"/>
<pad name="1" x="-8.89" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="2" x="-6.35" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="3" x="-3.81" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="4" x="-1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="5" x="1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="6" x="3.81" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="7" x="6.35" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="8" x="8.89" y="0" drill="1.016" shape="long" rot="R90"/>
<text x="-10.16" y="1.651" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-9.398" y="-2.921" size="1.27" layer="21" ratio="10">1</text>
<text x="8.255" y="1.651" size="1.27" layer="21" ratio="10">8</text>
<text x="-1.27" y="-2.921" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-6.604" y1="-0.254" x2="-6.096" y2="0.254" layer="51"/>
<rectangle x1="-9.144" y1="-0.254" x2="-8.636" y2="0.254" layer="51"/>
<rectangle x1="-4.064" y1="-0.254" x2="-3.556" y2="0.254" layer="51"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="3.556" y1="-0.254" x2="4.064" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
<rectangle x1="6.096" y1="-0.254" x2="6.604" y2="0.254" layer="51"/>
<rectangle x1="8.636" y1="-0.254" x2="9.144" y2="0.254" layer="51"/>
</package>
<package name="MA06-1">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-6.985" y1="1.27" x2="-5.715" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="1.27" x2="-5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-0.635" x2="-5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="0.635" x2="-4.445" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="1.27" x2="-3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="1.27" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-0.635" x2="-3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-1.27" x2="-4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="-1.27" x2="-5.08" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="0.635" x2="-7.62" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="1.27" x2="-7.62" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="-0.635" x2="-6.985" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="-1.27" x2="-6.985" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="1.27" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.635" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.27" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.27" x2="-2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-0.635" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="1.27" x2="4.445" y2="1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="1.27" x2="5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-0.635" x2="4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="-1.27" x2="3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-1.27" x2="2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.635" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.08" y1="0.635" x2="5.715" y2="1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="1.27" x2="6.985" y2="1.27" width="0.1524" layer="21"/>
<wire x1="6.985" y1="1.27" x2="7.62" y2="0.635" width="0.1524" layer="21"/>
<wire x1="7.62" y1="-0.635" x2="6.985" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="6.985" y1="-1.27" x2="5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="-1.27" x2="5.08" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="7.62" y1="0.635" x2="7.62" y2="-0.635" width="0.1524" layer="21"/>
<pad name="1" x="-6.35" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="2" x="-3.81" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="3" x="-1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="4" x="1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="5" x="3.81" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="6" x="6.35" y="0" drill="1.016" shape="long" rot="R90"/>
<text x="-7.62" y="1.651" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-6.985" y="-2.921" size="1.27" layer="21" ratio="10">1</text>
<text x="5.715" y="1.651" size="1.27" layer="21" ratio="10">6</text>
<text x="-2.54" y="-2.921" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-4.064" y1="-0.254" x2="-3.556" y2="0.254" layer="51"/>
<rectangle x1="-6.604" y1="-0.254" x2="-6.096" y2="0.254" layer="51"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="3.556" y1="-0.254" x2="4.064" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
<rectangle x1="6.096" y1="-0.254" x2="6.604" y2="0.254" layer="51"/>
</package>
<package name="MA05-1">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-5.715" y1="1.27" x2="-4.445" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="1.27" x2="-3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-0.635" x2="-4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="0.635" x2="-3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="1.27" x2="-1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-0.635" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.27" x2="-3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-1.27" x2="-3.81" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="0.635" x2="-6.35" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="1.27" x2="-6.35" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="-0.635" x2="-5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="-1.27" x2="-5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.635" x2="-0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="1.27" x2="0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-0.635" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-1.27" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.27" x2="-1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="1.27" x2="3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-0.635" x2="3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="3.81" y1="0.635" x2="4.445" y2="1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="1.27" x2="5.715" y2="1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="1.27" x2="6.35" y2="0.635" width="0.1524" layer="21"/>
<wire x1="6.35" y1="0.635" x2="6.35" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="6.35" y1="-0.635" x2="5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="-1.27" x2="4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="-1.27" x2="3.81" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-0.635" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-1.27" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<pad name="1" x="-5.08" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="2" x="-2.54" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="3" x="0" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="4" x="2.54" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="5" x="5.08" y="0" drill="1.016" shape="long" rot="R90"/>
<text x="-6.35" y="1.651" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-5.715" y="-2.921" size="1.27" layer="21" ratio="10">1</text>
<text x="4.445" y="1.651" size="1.27" layer="21" ratio="10">5</text>
<text x="-2.54" y="-2.921" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-2.794" y1="-0.254" x2="-2.286" y2="0.254" layer="51"/>
<rectangle x1="-5.334" y1="-0.254" x2="-4.826" y2="0.254" layer="51"/>
<rectangle x1="-0.254" y1="-0.254" x2="0.254" y2="0.254" layer="51"/>
<rectangle x1="4.826" y1="-0.254" x2="5.334" y2="0.254" layer="51"/>
<rectangle x1="2.286" y1="-0.254" x2="2.794" y2="0.254" layer="51"/>
</package>
</packages>
<symbols>
<symbol name="MA08-1">
<wire x1="3.81" y1="-10.16" x2="-1.27" y2="-10.16" width="0.4064" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-5.08" x2="2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-7.62" x2="2.54" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="1.27" y1="2.54" x2="2.54" y2="2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="1.27" y1="10.16" x2="2.54" y2="10.16" width="0.6096" layer="94"/>
<wire x1="1.27" y1="7.62" x2="2.54" y2="7.62" width="0.6096" layer="94"/>
<wire x1="1.27" y1="5.08" x2="2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="12.7" x2="-1.27" y2="-10.16" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-10.16" x2="3.81" y2="12.7" width="0.4064" layer="94"/>
<wire x1="-1.27" y1="12.7" x2="3.81" y2="12.7" width="0.4064" layer="94"/>
<text x="-1.27" y="-12.7" size="1.778" layer="96">&gt;VALUE</text>
<text x="-1.27" y="13.462" size="1.778" layer="95">&gt;NAME</text>
<pin name="1" x="7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="2" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="3" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="4" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="5" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="6" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="7" x="7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="8" x="7.62" y="10.16" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
</symbol>
<symbol name="MA06-1">
<wire x1="3.81" y1="-10.16" x2="-1.27" y2="-10.16" width="0.4064" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-5.08" x2="2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-7.62" x2="2.54" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="1.27" y1="2.54" x2="2.54" y2="2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="1.27" y1="5.08" x2="2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="7.62" x2="-1.27" y2="-10.16" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-10.16" x2="3.81" y2="7.62" width="0.4064" layer="94"/>
<wire x1="-1.27" y1="7.62" x2="3.81" y2="7.62" width="0.4064" layer="94"/>
<text x="-1.27" y="-12.7" size="1.778" layer="96">&gt;VALUE</text>
<text x="-1.27" y="8.382" size="1.778" layer="95">&gt;NAME</text>
<pin name="1" x="7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="2" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="3" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="4" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="5" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="6" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
</symbol>
<symbol name="MA05-1">
<wire x1="3.81" y1="-7.62" x2="-1.27" y2="-7.62" width="0.4064" layer="94"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-5.08" x2="2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="7.62" x2="-1.27" y2="-7.62" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-7.62" x2="3.81" y2="7.62" width="0.4064" layer="94"/>
<wire x1="-1.27" y1="7.62" x2="3.81" y2="7.62" width="0.4064" layer="94"/>
<wire x1="1.27" y1="5.08" x2="2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="1.27" y1="2.54" x2="2.54" y2="2.54" width="0.6096" layer="94"/>
<text x="-1.27" y="-10.16" size="1.778" layer="96">&gt;VALUE</text>
<text x="-1.27" y="8.382" size="1.778" layer="95">&gt;NAME</text>
<pin name="1" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="2" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="3" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="4" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="5" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="MA08-1" prefix="SV" uservalue="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="MA08-1" x="0" y="0"/>
</gates>
<devices>
<device name="" package="MA08-1">
<connects>
<connect gate="1" pin="1" pad="1"/>
<connect gate="1" pin="2" pad="2"/>
<connect gate="1" pin="3" pad="3"/>
<connect gate="1" pin="4" pad="4"/>
<connect gate="1" pin="5" pad="5"/>
<connect gate="1" pin="6" pad="6"/>
<connect gate="1" pin="7" pad="7"/>
<connect gate="1" pin="8" pad="8"/>
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
<deviceset name="MA06-1" prefix="SV" uservalue="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="MA06-1" x="0" y="0"/>
</gates>
<devices>
<device name="" package="MA06-1">
<connects>
<connect gate="1" pin="1" pad="1"/>
<connect gate="1" pin="2" pad="2"/>
<connect gate="1" pin="3" pad="3"/>
<connect gate="1" pin="4" pad="4"/>
<connect gate="1" pin="5" pad="5"/>
<connect gate="1" pin="6" pad="6"/>
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
<deviceset name="MA05-1" prefix="SV" uservalue="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="MA05-1" x="0" y="0"/>
</gates>
<devices>
<device name="" package="MA05-1">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
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
<part name="U$1" library="kostka" deviceset="KOSTKA_2.5MM_X8" device=""/>
<part name="U$3" library="kostka" deviceset="KOSTKA_2.5MM_X6" device=""/>
<part name="U$6" library="kostka" deviceset="KOSTKA_2.5MM_X5" device=""/>
<part name="OUT" library="con-lstb" deviceset="MA08-1" device=""/>
<part name="IN" library="con-lstb" deviceset="MA06-1" device=""/>
<part name="LCD" library="con-lstb" deviceset="MA05-1" device=""/>
<part name="U$2" library="kostka" deviceset="KOSTKA_2.5MM_X5" device=""/>
<part name="U$4" library="kostka" deviceset="KOSTKA_2.5MM_X5" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$1" gate="G$1" x="80.01" y="16.51"/>
<instance part="U$3" gate="G$1" x="101.6" y="12.7" rot="R180"/>
<instance part="U$6" gate="G$1" x="80.01" y="-7.62"/>
<instance part="OUT" gate="1" x="60.96" y="15.24"/>
<instance part="IN" gate="1" x="119.38" y="17.78" rot="R180"/>
<instance part="LCD" gate="G$1" x="60.96" y="-7.62"/>
<instance part="U$2" gate="G$1" x="97.79" y="-2.54" rot="R180"/>
<instance part="U$4" gate="G$1" x="97.79" y="-22.86" rot="R180"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<pinref part="OUT" gate="1" pin="8"/>
<wire x1="71.12" y1="25.4" x2="68.58" y2="25.4" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="P$1"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="OUT" gate="1" pin="7"/>
<pinref part="U$1" gate="G$1" pin="P$2"/>
<wire x1="68.58" y1="22.86" x2="71.12" y2="22.86" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="OUT" gate="1" pin="6"/>
<pinref part="U$1" gate="G$1" pin="P$3"/>
<wire x1="68.58" y1="20.32" x2="71.12" y2="20.32" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="OUT" gate="1" pin="5"/>
<pinref part="U$1" gate="G$1" pin="P$4"/>
<wire x1="68.58" y1="17.78" x2="71.12" y2="17.78" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="OUT" gate="1" pin="4"/>
<pinref part="U$1" gate="G$1" pin="P$5"/>
<wire x1="68.58" y1="15.24" x2="71.12" y2="15.24" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="OUT" gate="1" pin="3"/>
<pinref part="U$1" gate="G$1" pin="P$6"/>
<wire x1="68.58" y1="12.7" x2="71.12" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="OUT" gate="1" pin="2"/>
<pinref part="U$1" gate="G$1" pin="P$7"/>
<wire x1="68.58" y1="10.16" x2="71.12" y2="10.16" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="OUT" gate="1" pin="1"/>
<pinref part="U$1" gate="G$1" pin="P$8"/>
<wire x1="68.58" y1="7.62" x2="71.12" y2="7.62" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<pinref part="LCD" gate="G$1" pin="1"/>
<wire x1="68.58" y1="-12.7" x2="71.12" y2="-12.7" width="0.1524" layer="91"/>
<pinref part="U$6" gate="G$1" pin="P$5"/>
</segment>
</net>
<net name="N$24" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="P$6"/>
<pinref part="IN" gate="1" pin="1"/>
<wire x1="106.68" y1="25.4" x2="111.76" y2="25.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$25" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="P$5"/>
<pinref part="IN" gate="1" pin="2"/>
<wire x1="106.68" y1="22.86" x2="111.76" y2="22.86" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$26" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="P$4"/>
<pinref part="IN" gate="1" pin="3"/>
<wire x1="106.68" y1="20.32" x2="111.76" y2="20.32" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$27" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="P$3"/>
<pinref part="IN" gate="1" pin="4"/>
<wire x1="106.68" y1="17.78" x2="111.76" y2="17.78" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$28" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="P$2"/>
<pinref part="IN" gate="1" pin="5"/>
<wire x1="106.68" y1="15.24" x2="111.76" y2="15.24" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$29" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="P$1"/>
<pinref part="IN" gate="1" pin="6"/>
<wire x1="106.68" y1="12.7" x2="111.76" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$36" class="0">
<segment>
<pinref part="LCD" gate="G$1" pin="2"/>
<pinref part="U$6" gate="G$1" pin="P$4"/>
<wire x1="68.58" y1="-10.16" x2="71.12" y2="-10.16" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$37" class="0">
<segment>
<pinref part="LCD" gate="G$1" pin="3"/>
<pinref part="U$6" gate="G$1" pin="P$3"/>
<wire x1="68.58" y1="-7.62" x2="71.12" y2="-7.62" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$38" class="0">
<segment>
<pinref part="LCD" gate="G$1" pin="4"/>
<pinref part="U$6" gate="G$1" pin="P$2"/>
<wire x1="68.58" y1="-5.08" x2="71.12" y2="-5.08" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$39" class="0">
<segment>
<pinref part="LCD" gate="G$1" pin="5"/>
<pinref part="U$6" gate="G$1" pin="P$1"/>
<wire x1="68.58" y1="-2.54" x2="71.12" y2="-2.54" width="0.1524" layer="91"/>
</segment>
</net>
<net name="VCC" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="P$1"/>
<wire x1="109.22" y1="-7.62" x2="106.68" y2="-7.62" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="P$2"/>
<wire x1="109.22" y1="-5.08" x2="106.68" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="109.22" y1="-7.62" x2="109.22" y2="-5.08" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="P$3"/>
<wire x1="109.22" y1="-2.54" x2="106.68" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="109.22" y1="-5.08" x2="109.22" y2="-2.54" width="0.1524" layer="91"/>
<junction x="109.22" y="-5.08"/>
<pinref part="U$2" gate="G$1" pin="P$4"/>
<wire x1="109.22" y1="0" x2="106.68" y2="0" width="0.1524" layer="91"/>
<wire x1="109.22" y1="-2.54" x2="109.22" y2="0" width="0.1524" layer="91"/>
<junction x="109.22" y="-2.54"/>
<wire x1="109.22" y1="2.54" x2="106.68" y2="2.54" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="P$5"/>
<wire x1="109.22" y1="0" x2="109.22" y2="2.54" width="0.1524" layer="91"/>
<junction x="109.22" y="0"/>
<wire x1="109.22" y1="-2.54" x2="119.38" y2="-2.54" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<wire x1="109.22" y1="-17.78" x2="106.68" y2="-17.78" width="0.1524" layer="91"/>
<pinref part="U$4" gate="G$1" pin="P$5"/>
<pinref part="U$4" gate="G$1" pin="P$4"/>
<wire x1="109.22" y1="-20.32" x2="106.68" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="109.22" y1="-17.78" x2="109.22" y2="-20.32" width="0.1524" layer="91"/>
<pinref part="U$4" gate="G$1" pin="P$3"/>
<wire x1="109.22" y1="-22.86" x2="106.68" y2="-22.86" width="0.1524" layer="91"/>
<wire x1="109.22" y1="-20.32" x2="109.22" y2="-22.86" width="0.1524" layer="91"/>
<junction x="109.22" y="-20.32"/>
<pinref part="U$4" gate="G$1" pin="P$2"/>
<wire x1="109.22" y1="-25.4" x2="106.68" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="109.22" y1="-22.86" x2="109.22" y2="-25.4" width="0.1524" layer="91"/>
<junction x="109.22" y="-22.86"/>
<pinref part="U$4" gate="G$1" pin="P$1"/>
<wire x1="109.22" y1="-27.94" x2="106.68" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="109.22" y1="-25.4" x2="109.22" y2="-27.94" width="0.1524" layer="91"/>
<junction x="109.22" y="-25.4"/>
<wire x1="109.22" y1="-22.86" x2="119.38" y2="-22.86" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
