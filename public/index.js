let input = document.querySelector("#hours");
let select = document.querySelector("#months");
function clickHours() 
{
    input.disabled = false;
    select.disabled = true;
}

function clickMonths()
{
    input.disabled = true;
    select.disabled = false;
}