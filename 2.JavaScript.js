    function expandAndCollapse(id, btn)
    {
        var tag2 = document.getElementById(btn);
        var tag = document.getElementById(id);
        if(tag.style.display == "none")
        {
            tag2.innerText = "-";
            tag.style.display = "block";
        }
        else
        {
            tag2.innerText = "+";
            tag.style.display = "none";
        }
/*        if (tag2.innerText == "+") {
            tag2.innerText = "-";
        }
        else { tag2.innerText = "+"; } */
    }