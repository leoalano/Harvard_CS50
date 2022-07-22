function check() {
    var points=0;
    var q1=document.quiz1.quantidade.value;
    var q2=document.quiz1.atual.value;
    var q3=document.quiz1.dinheiro.value;
    var q4=document.quiz1.agregado.value;
    var q5=document.quiz1.ouro.value;
    var q6=document.quiz1.mineradores.value;
    var q7=document.quiz1.preço.value;


    if (q1=="quant2") {points++}
    if (q2=="atual3") {points++}
    if (q3=="dinheiro1") {points++}
    if (q4=="agregado3") {points++}
    if (q5=="ouro4") {points++}
    if (q6=="mineradores1") {points++}
    if (q7=="preço4") {points++}

    if (points<5) {
        document.getElementById("result").innerHTML = points/7*100 + "% das respostas corretas." + "<br>" +
        "Você pode melhorar, continue estudando";
      }
    else {
        document.getElementById("result").innerHTML = points/7*100 + "% das respostas corretas." + "<br>" +
        "Parabéns!! Continue estudando";
    }


}