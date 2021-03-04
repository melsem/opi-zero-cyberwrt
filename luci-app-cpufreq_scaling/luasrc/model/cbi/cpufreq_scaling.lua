local fs = require "nixio.fs"
local m, s

cpu_freqs = fs.readfile("/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies") or "1008000"
cpu_freqs = string.sub(cpu_freqs, 1, -3)
cpu_governors = fs.readfile("/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors") or "performance"
cpu_governors = string.sub(cpu_governors, 1, -3)

function string.split(input, delimiter)
    input = tostring(input)
    delimiter = tostring(delimiter)
    if (delimiter=='') then return false end
    local pos,arr = 0, {}
    for st,sp in function() return string.find(input, delimiter, pos, true) end do
        table.insert(arr, string.sub(input, pos, st - 1))
        pos = sp + 1
    end
    table.insert(arr, string.sub(input, pos))
    return arr
end

freq_array = string.split(cpu_freqs, " ")
governor_array = string.split(cpu_governors, " ")

m = Map("cpufreq_scaling", translate("CPU Freq Settings"))
s = m:section(NamedSection, "cpufreq", "cpufreq")
s.anonymouse = true
s.description = translate("Set CPU Scaling Governor to Max Performance or Balance Mode")

governor = s:option(ListValue, "governor", translate("CPU Scaling Governor"))
for _, e in ipairs(governor_array) do
	if e ~= "" then governor:value(translate(e,string.upper(e))) end
end

minifreq = s:option(ListValue, "min_freq", translate("Min Idle CPU Freq"))
for _, e in ipairs(freq_array) do
	if e ~= "" then minifreq:value(e) end
end

maxfreq = s:option(ListValue, "max_freq", translate("Max Turbo Boost CPU Freq"))
for _, e in ipairs(freq_array) do
	if e ~= "" then maxfreq:value(e) end
end

upthreshold = s:option(Value, "upthreshold", translate("CPU Switching Threshold"))
upthreshold.datatype="range(1,99)"
upthreshold.rmempty = false
upthreshold.description = translate("Kernel make a decision on whether it should increase the frequency (%)")
upthreshold.placeholder = 50
upthreshold.default = 50

factor = s:option(Value, "factor", translate("CPU Switching Sampling rate"))
factor.datatype="range(1,100000)"
factor.rmempty = false
factor.description = translate("The sampling rate determines how frequently the governor checks to tune the CPU (ms)")
factor.placeholder = 10
factor.default = 10

return m

