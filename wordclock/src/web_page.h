#ifndef WORDCLOCK_WEB_PAGE_H
#define WORDCLOCK_WEB_PAGE_H

#include <pgmspace.h>

// Static page, sent straight from flash. It loads its values from
// GET /api/config and saves them with PUT /api/config.
static const char INDEX_HTML[] PROGMEM = R"html(<!DOCTYPE html>
<html lang="de"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Wordclock</title><link rel="icon" href="data:,">
<style>
body{font-family:system-ui,sans-serif;background:#eee;color:#333;max-width:32em;margin:0 auto;padding:1em}
h1{color:#3d72a8;font-size:1.4em}h1 small{font-weight:normal;color:#777}
fieldset{border:0;background:#fff;border-radius:8px;margin:0 0 1em;padding:.6em 1em}
legend{font-weight:bold;color:#3d72a8;padding:0;float:left;width:100%}
label{display:flex;align-items:center;gap:.8em;margin:.7em 0;clear:both}
label>span{flex:0 0 9em}label>input:not([type=checkbox]),label>select{flex:1;min-width:0;font:inherit}
output{flex:0 0 3em;text-align:right}p{color:#777;font-size:.85em;margin:.3em 0}
button{font:inherit;background:#3d72a8;color:#fff;border:0;border-radius:6px;padding:.7em 1.2em}
.danger{background:#a83d3d}#msg{margin-left:1em}
</style></head><body>
<h1>Wordclock <small id="clock"></small></h1>
<form id="f">
<fieldset><legend>Uhr</legend>
<label><span>Vordergrund</span><input type="color" name="fg"></label>
<label><span>Hintergrund</span><input type="color" name="bg"></label>
<label><span>Helligkeit</span><input type="range" name="brightness" min="0" max="100" step="5"><output></output></label>
</fieldset>
<fieldset><legend>Halo</legend>
<label><span>Aktiv</span><input type="checkbox" name="halo_on"></label>
<label><span>Farbe</span><input type="color" name="halo"></label>
<label><span>Helligkeit</span><input type="range" name="halo_brightness" min="0" max="100" step="5"><output></output></label>
<label><span>Netzteil (mA)</span><input type="number" name="power_supply" min="0" step="100"></label>
<p id="halo_max"></p>
</fieldset>
<fieldset><legend>Zeit</legend>
<label><span>Zeitzone</span><select id="tz_preset">
<option value="CET-1CEST,M3.5.0,M10.5.0/3">MEZ/MESZ automatisch</option>
<option value="CET-1">MEZ fest (immer Winterzeit)</option>
<option value="CEST-2">MESZ fest (immer Sommerzeit)</option>
<option value="">Eigene Regel</option></select></label>
<label><span>POSIX-TZ-Regel</span><input type="text" name="tz" maxlength="47"></label>
<label><span>NTP-Server</span><input type="text" name="ntp" maxlength="47"></label>
</fieldset>
<fieldset><legend>Nachtmodus</legend>
<label><span>Aktiv</span><input type="checkbox" name="dnd_on"></label>
<label><span>Beginn</span><input type="time" name="dnd_start" step="300"></label>
<label><span>Ende</span><input type="time" name="dnd_end" step="300"></label>
</fieldset>
<fieldset><legend>System</legend>
<label><span>LED-Test beim Start</span><input type="checkbox" name="healthcheck"></label>
<p id="version"></p>
</fieldset>
<button>Speichern</button><span id="msg"></span>
</form>
<fieldset style="margin-top:2em"><legend>WLAN</legend>
<p>Löscht die WLAN-Zugangsdaten. Danach öffnet die Uhr den Hotspot „WordClock“ zum Einrichten.</p>
<button class="danger" id="wifi">WLAN-Einstellungen löschen</button>
</fieldset>
<script>
const f=document.getElementById('f'),$=id=>document.getElementById(id);
function outputs(){for(const r of f.querySelectorAll('input[type=range]'))r.nextElementSibling.value=r.value+' %';}
function show(c){
 for(const el of f.elements){if(!(el.name in c))continue;
  if(el.type=='checkbox')el.checked=c[el.name];else el.value=c[el.name];}
 const p=$('tz_preset');p.value=c.tz;if(p.value!=c.tz)p.value='';
 $('halo_max').textContent='Mit diesem Netzteil sind höchstens '+c.halo_max+' % Halo-Helligkeit möglich.';
 $('clock').textContent=c.time||'Zeit noch nicht synchronisiert';
 $('version').textContent='Version '+c.version;
 outputs();}
f.oninput=outputs;
$('tz_preset').onchange=e=>{if(e.target.value)f.tz.value=e.target.value;};
fetch('/api/config').then(r=>r.json()).then(show);
f.onsubmit=e=>{e.preventDefault();const c={};
 for(const el of f.elements){if(!el.name)continue;
  c[el.name]=el.type=='checkbox'?el.checked:(el.type=='range'||el.type=='number')?Number(el.value):el.value;}
 $('msg').textContent='…';
 fetch('/api/config',{method:'PUT',headers:{'Content-Type':'application/json'},body:JSON.stringify(c)})
  .then(r=>r.ok?r.json():Promise.reject()).then(c=>{show(c);$('msg').textContent='Gespeichert';})
  .catch(()=>$('msg').textContent='Fehler beim Speichern');};
$('wifi').onclick=()=>{if(confirm('WLAN-Zugangsdaten wirklich löschen?'))
 fetch('/api/wifi/reset',{method:'POST'}).then(()=>document.body.textContent='WLAN gelöscht. Bitte mit dem Hotspot „WordClock“ verbinden.');};
</script></body></html>)html";

#endif
