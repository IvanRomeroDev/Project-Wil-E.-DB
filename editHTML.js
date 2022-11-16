
const region_sr = document.getElementById('region_sr')
const space_sr = document.getElementById('space_sr')
const vcpu_sr = document.getElementById('vcpu_sr')
const shards_sr = document.getElementById('shards_sr')
const nodes_sr = document.getElementById('nodes_sr')
const billing_sr = document.getElementById('billing_sr')
const period_sr = document.getElementById('period_sr')
const cost_sr = document.getElementById('cost_sr')

region_sr.innerHTML += regionSR
space_sr.innerHTML += storageSR + " GB"
vcpu_sr.innerHTML += vCPUSR
shards_sr.innerHTML += shardsSR
nodes_sr.innerHTML += CNSR
if(monthsSR == undefined){
    billing_sr.innerHTML += "hourly"
    period_sr.innerHTML += hoursSR + "hours"
} else if(monthsSR == 12){
    billing_sr.innerHTML += "yearly"
    period_sr.innerHTML += 1 + " year"
} else {
    billing_sr.innerHTML += "monthly"
    period_sr.innerHTML += monthsSR + " months"
}

cost_sr.innerHTML += "$" + costSR + " USD"