package components

import (
	"fmt"
	"math/rand"
	"strconv"

	"github.com/charmbracelet/bubbles/progress"
	"github.com/charmbracelet/lipgloss"
)

type InfoBox struct {
	usage         int
	cores         int
	cores_usage   []int
	width, height int
	loadAvg       float32
	temp          float32
	name          string
	freq_mhz      float32

	progress_total progress.Model
}

func (i *InfoBox) SetSize(w, h int) {
	i.width = w
	i.height = h
}

func (i *InfoBox) SetUsage(u int) {
	i.usage = u
}

func (i *InfoBox) SetCores(c int) {
	i.cores = c
}

func (i *InfoBox) SetCoresUsage(a []int) {
	i.cores_usage = a
}

func (i InfoBox) View() string {
	i.temp = rand.Float32()
	i.loadAvg = rand.Float32()
	i.name = "i5-7500U"
	i.freq_mhz = float32(3.1)

	usageStr := fmt.Sprintf("CPU Model %.10s\tFreq -- %.1fMHz\n", i.name, i.freq_mhz)
	usageStr += fmt.Sprintf("loadAvg  %.2f\tTemp -- %.fC\n", i.loadAvg, i.temp)

	i.progress_total.Width = i.width - 8
	usageStr = usageStr + i.progress_total.ViewAs(float64(i.usage)/100.0) + strconv.Itoa(i.usage) + "%"

	// usageStr += fmt.Sprintf("CPU Usage: %d%%\n", c.usage)
	// usageStr += fmt.Sprintf("CPU Total: %s\n", c.progress.ViewAs(float64(i.usage)/100.0))

	boxStyle := lipgloss.NewStyle().Width(i.width).
		Height(i.height).
		Border(lipgloss.NormalBorder()).
		Align(lipgloss.Center).
		Padding(0, 1)

	return boxStyle.Render(usageStr)
}

func NewInfoBox() *InfoBox {
	p := progress.New(progress.WithDefaultGradient())
	p.ShowPercentage = false

	return &InfoBox{
		usage:          80,
		name:           "i5-7200U",
		freq_mhz:       2000,
		loadAvg:        35.0,
		temp:           39,
		progress_total: p,
	}
}
