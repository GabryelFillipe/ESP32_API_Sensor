const express = require("express")
const app = express()

let dados = []

app.get("/sensor", (req,res) =>{
    const {temp,hum} = req.query

    if (!temp || !hum) {
        return res.status(400).send("dados invalidos")
    }

    const dataFormatada = new Date().toLocaleString('pt-BR', { 
        timeZone: 'America/Sao_Paulo' 
    })

    const leitura = {
        temperatura: temp,
        umidade: hum,
        data: dataFormatada
    }

    dados.push(leitura)

    console.log(leitura)

    res.send("Dados Captados")
})


app.get("/dados", (req,res) =>{
    res.json(dados)
})
const PORT = process.env.PORT || 3000

app.listen(PORT, function () {
    console.log('API Aguardando Requisições!!!')
})