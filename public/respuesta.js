const regionSR = "LA-Mexico-City2"
const vCPUSR = "8 vCPUs"
const shardsSR = 1
const CNSR = 1
const storageSR = 160
const monthsSR = 0
const hoursSR = 1
const costSR = 13.57

const regionBC = "CN-Southwest-Guiyang1"
const vCPUBC = "8 vCPUs"
const shardsBC = 1
const CNBC = 1
const storageBC = 160
const monthsBC = 0
const hoursBC = 1.00
const costBC = 12.92

const regionB = "CN-Southwest-Guiyang1"
const vCPUB = "8 vCPUs"
const shardsB = 1
const CNB = 1
const storageB = 160
const monthsB = 0
const hoursB = 1.00
const costB = 13.68


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
if(monthsSR == 0){
    billing_sr.innerHTML += " hourly"
    period_sr.innerHTML += hoursSR + " hours"
} else if(monthsSR == 12){
    billing_sr.innerHTML += " yearly"
    period_sr.innerHTML += 1 + " year"
} else {
    billing_sr.innerHTML += "monthly"
    period_sr.innerHTML += monthsSR + " months"
}
cost_sr.innerHTML += "$" + costSR + " USD"


const region_bc = document.getElementById('region_bc')
const space_bc = document.getElementById('space_bc')
const vcpu_bc = document.getElementById('vcpu_bc')
const shards_bc = document.getElementById('shards_bc')
const nodes_bc = document.getElementById('nodes_bc')
const billing_bc = document.getElementById('billing_bc')
const period_bc = document.getElementById('period_bc')
const cost_bc = document.getElementById('cost_bc')

region_bc.innerHTML += regionBC
space_bc.innerHTML += storageBC + " GB"
vcpu_bc.innerHTML += vCPUBC
shards_bc.innerHTML += shardsBC
nodes_bc.innerHTML += CNBC
if(monthsBC == 0){
    billing_bc.innerHTML += " hourly"
    period_bc.innerHTML += hoursBC + " hours"
} else if(monthsBC == 12){
    billing_bc.innerHTML += " yearly"
    period_bc.innerHTML += 1 + " year"
} else {
    billing_bc.innerHTML += " monthly"
    period_bc.innerHTML += monthsBC + " months"
}
cost_bc.innerHTML += "$" + costBC + " USD"


const region_b = document.getElementById('region_b')
const space_b = document.getElementById('space_b')
const vcpu_b = document.getElementById('vcpu_b')
const shards_b = document.getElementById('shards_b')
const nodes_b = document.getElementById('nodes_b')
const billing_b = document.getElementById('billing_b')
const period_b = document.getElementById('period_b')
const cost_b = document.getElementById('cost_b')

region_b.innerHTML += regionB
space_b.innerHTML += storageB + " GB"
vcpu_b.innerHTML += vCPUB
shards_b.innerHTML += shardsB
nodes_b.innerHTML += CNB
if(monthsB == 0){
    billing_b.innerHTML += " hourly"
    period_b.innerHTML += hoursB + " hours"
} else if(monthsB == 12){
    billing_b.innerHTML += " yearly"
    period_b.innerHTML += 1 + " year"
} else {
    billing_b.innerHTML += " monthly"
    period_b.innerHTML += monthsB + " months"
}

cost_b.innerHTML += "$" + costB + " USD"
