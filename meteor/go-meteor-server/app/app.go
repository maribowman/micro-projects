package app

import (
	"fmt"
	"github.com/gin-gonic/gin"
	ginPrometheus "github.com/zsais/go-gin-prometheus"
	"go-meteor-server/app/config"
	"net/http"
)

func InitRouter() *http.Server {
	return &http.Server{
		Addr:    fmt.Sprintf(":%d", config.Config.Server.Port),
		Handler: injectRouter(NewMeteorService(&Wiring{})),
	}
}

func injectRouter(service MeteorService) *gin.Engine {
	gin.SetMode(config.Config.Server.Mode)
	router := gin.Default()
	ginPrometheus.NewPrometheus("meteor_server").Use(router)

	router.POST("/v1/meteor", service.PostData)
	router.GET("/v1/meteor", service.GetData)
	router.GET("/v1/times", service.GetTimes)

	return router
}
