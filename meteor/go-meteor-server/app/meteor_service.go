package app

import (
	"github.com/gin-gonic/gin"
	"net/http"
	"time"
)

var (
	temperature float32
	humidity    float32
	seen        = false
	firstSeen   time.Time
	lastSeen    time.Time
)

type Data struct {
	Temperature float32 `json:"temperature" binding:"required"`
	Humidity    float32 `json:"humidity" binding:"required"`
	Pressure    float32 `json:"pressure"`
}

type MeteorService struct {
}

type Wiring struct {
}

func NewMeteorService(wiring *Wiring) MeteorService {
	return MeteorService{}
}

func (service *MeteorService) PostData(context *gin.Context) {
	var data Data
	if err := context.ShouldBindJSON(&data); err != nil {
		context.Status(http.StatusBadRequest)
		return
	}
	temperature = data.Temperature
	humidity = data.Humidity
	if !seen {
		firstSeen = time.Now()
		seen = true
	}
	lastSeen = time.Now()
	context.Status(http.StatusCreated)
}

func (service *MeteorService) GetData(context *gin.Context) {
	context.JSON(http.StatusOK, Data{
		Temperature: temperature,
		Humidity:    humidity,
	})
}

func (service *MeteorService) GetTimes(context *gin.Context) {
	context.JSON(http.StatusOK, gin.H{"firstSeen": firstSeen,
		"lastSeen": lastSeen,
		"uptime":   lastSeen.Sub(firstSeen).Hours(),
	})
}
