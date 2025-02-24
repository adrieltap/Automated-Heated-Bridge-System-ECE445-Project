# Kahmil Worklog

[[_TOC_]]

## Questions

1. How many watts would we need to increase the temperature of the metal plate by 10 degrees?
2. Any guidance on the power subsystem? How do we connect to the wall?
3. Do we need a display for temperature and moisture?
4. Talk to machine shop about how the demo would look like and using a cooler with dry ice for demo
5. Do we need heat sinks?
6. What does the breadboard demonstration in week 8 look like exactly?
7. If our power supply looks like a laptop charger, how do we connect it to our system?
8. What will be the heat capacity of the material used for the surface?

## 2/24/2025 - Meeting with machine shop/Parts for Power subsystem

### Parts for Power subsystem

Workflow: **Wall Outlet** -> **AC Power cord** -> **AC-DC converter (120V -> 24V)** -> **Buck Converter (24V -> 5V)**

- AC Power Cord
  - Option 1: Pre-stripped power cord [(https://www.amazon.com/Pigtail-3-Wire-Stripped-Supply-Replacement/dp/B0829LSF2F)](https://www.amazon.com/Pigtail-3-Wire-Stripped-Supply-Replacement/dp/B0829LSF2F) but safety concerns
  - Option 2: cut and strip a regular power cord to expose the three wires
  - Might need a fuse

- AC-DC converter (120V -> 24V)
  - [280W,24V](https://www.sager.com/product/gst280a24-c6p-mean-well/5359901?utm_source=googlemerchant&utm_medium=click&utm_campaign=sager-brand&utm_source=microsoft-ads&utm_medium=cpc&campaign-name=SmartShopping-NEW&utm_agid=1319416408946534&utm_term=&creative=&device=c&placement=&msclkid=8bbfccc8aff917a70a6fab7421873c48&utm_content=Ad%20group%20%231)
  - [150W,24V](https://www.sager.com/product/lrs-150-24-mean-well/5396767?utm_source=microsoft-ads&campaign-name=Search-Sager&Power&utm_agid=1313918793376081&utm_term=product%20&creative=&device=c&placement=&msclkid=6cfe252aa0721c05c83a42962f210577&utm_medium=cpc&utm_campaign=Search%20-%20Sager%20%26%20Power&utm_content=Electronics%20-%20Dynamic)
  -[120W,24V](https://trcelectronics.com/products/mean-well-gst120a24-r7b?utm_term=&utm_campaign=Sales-Performance+Max-HIGH&utm_source=bing&utm_medium=ppc&hsa_acc=9805199880&hsa_cam=20129401809&hsa_grp=1271037090758778&hsa_ad=&hsa_src=o&hsa_tgt=pla-2331239529291898&hsa_kw=&hsa_mt=e&hsa_net=adwords&hsa_ver=3&msclkid=1578a6364dad1e7983c9c18add5d6922&utm_content=Asset%20Group%201)
  -[144W,24V(top option)](https://www.amazon.com/gp/product/B0CFFBMQ6W/ref=sw_img_1?smid=A3OX6A2904ZV12&psc=1)

- Buck Converter (24V -> 5V)
  - [Option 1](https://www.amazon.com/Zixtec-LM2596-Converter-Module-1-25V-30V/dp/B07VVXF7YX/ref=sr_1_1_sspa?adgrpid=1338106215344359&dib=eyJ2IjoiMSJ9.i-fEUZUMI6-HUhAvIrsL29-8sAiPbBM2NOrzWmpKicRhKOJc89Q7s3C-mw-o6bP7y2_23eSVDDGrgKpZWt6iobrW6keTpDD50gEzcF3R3xl7l38HGH0awaiHHapeLC2wDxFhbknTLNMsKTJc1v7_PSi4KXp2FUUme7whoLVaP_lAaJXZVsZGvoEvoWVIcxYH8_4DXOMEVbF4WBjGEyH1p3xQSGN-ILX0teGRgqRg8ts.Cmr4-DlNpWikNU6o2kz8kpZiJgpuy1QVTUvAXkESvr0&dib_tag=se&hvadid=83631853424737&hvbmt=be&hvdev=c&hvlocphy=95058&hvnetw=o&hvqmt=e&hvtargid=kwd-83631983725861%3Aloc-190&hydadcr=18918_13351314&keywords=lm2596%2Bdc%2Bdc%2Bbuck%2Bconverter&mcid=adfbc3c395583070af1c1c8c87dfef6f&msclkid=ab1bf28172511d781971953de778885b&qid=1740412209&sr=8-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&th=1)

### Amount of Power Needed
We need to consider the amount of power neededby our system in order to choose the right power supply and other components.

## Safety

- Wall outlet voltage (120V AC or 230V AC) is deadly.
  - Always use insulated wires, power supplies, and fuses.
- Current above 30mA can be fatal.
  - Ensure proper insulation and grounding in your circuits.
- Use a fuse or circuit breaker to prevent excessive current flow.
  - Example: 5A fuse for AC input, 10A fuse for 24V heater circuit.
- Never work on live circuits.
  - Always unplug the system before making changes.
  Source: ChatGPT


- ![alt text](current_safety.png)
Source: https://www.scienceabc.com/humans/how-many-volts-amps-kill-you-human.html
