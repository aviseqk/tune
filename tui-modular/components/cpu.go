package components

import (
	"github.com/charmbracelet/bubbles/progress"
	"github.com/charmbracelet/lipgloss"
)

type BorderChars struct {
	top_left     string
	top_right    string
	bottom_left  string
	bottom_right string
	bar          string
	pipe         string
}

var myBorderChars = BorderChars{
	top_left:     "╭",
	top_right:    "╮",
	bottom_left:  "╰",
	bottom_right: "╯",
	bar:          "─",
	pipe:         "│",
}

type CPUBox struct {
	graph    *GraphBox
	info     *InfoBox
	width    int
	height   int
	usage    int
	name     string
	freq_mhz int
	progress progress.Model
}

func NewCPUBox() *CPUBox {
	p := progress.New(progress.WithDefaultGradient())
	p.ShowPercentage = false

	// var initGraphBox *components.InfoBox

	initGraphBox := NewGraphBox()
	initInfoBox := NewInfoBox()

	return &CPUBox{
		usage:    80,
		name:     "i5-7200U",
		freq_mhz: 2000,
		progress: p,
		graph:    initGraphBox,
		info:     initInfoBox,
	}
}

func (c *CPUBox) layoutSubComponents() {
	infoWidth := c.width / 4
	infoHeight := 3 * c.height / 4
	graphWidth := c.width - infoWidth

	c.graph.SetSize(graphWidth, c.height)
	c.info.SetSize(infoWidth, infoHeight)
}

func (c *CPUBox) SetSize(w, h int) {
	c.width = w
	c.height = h
}

func (c *CPUBox) SetUsage(percent int, name string, freq int) {
	c.usage = percent
	if name != "" {
		c.name = name
	}
	c.freq_mhz = freq

	c.graph.SetUsage(percent)
	c.info.SetUsage(percent)
}

func (c CPUBox) View() string {

	c.layoutSubComponents()

	return lipgloss.JoinHorizontal(lipgloss.Center,
		c.info.View(), c.graph.View())
	// barWidth := c.width
	// filled := (barWidth * c.usage) / 100
	// empty := barWidth - filled
	// bar := strings.Repeat("|", filled) + strings.Repeat("_", empty)
	// usageStr := strings.Repeat(" ", (barWidth*6)/10) +
	// usageStr := fmt.Sprintf("CPU Model %.10s\tFreq -- %dMHz\n", c.name, c.freq_mhz)
	// // usageStr += fmt.Sprintf("CPU Usage: %d%%\n", c.usage)
	// usageStr += fmt.Sprintf("CPU Total: %s\n", c.progress.ViewAs(float64(c.usage)/100.0))

	// boxStyle := lipgloss.NewStyle().Width(c.width).
	// 	Height(c.height).
	// 	Border(lipgloss.ASCIIBorder()).
	// 	Align(lipgloss.Center).
	// 	Padding(0, 1)

	// return boxStyle.Render(usageStr)
}
