package components

import (
	"fmt"
	"github.com/charmbracelet/lipgloss"
)

type InterfaceBox struct {
	info          *NetworkInfoBox
	graph         *NetworkGraphBox
	networkName   string
	ip            string
	width, height int
}

type NetworkBox struct {
	width, height int
	interfaces    map[string]*InterfaceBox
	selected      string
}

type NetStats struct {
	Name           string
	ip             string
	download_total float64
	download_speed float64
	download_top   float64
	upload_total   float64
	upload_speed   float64
	upload_top     float64
}

func (i *InterfaceBox) Update(stat NetStats) {
	i.networkName = stat.Name
	i.ip = stat.ip
	i.graph.Update(stat)
	i.info.Update(stat)
}

func (n *NetworkBox) Update(allStats map[string]NetStats) {
	for name, stat := range allStats {
		if iface, ok := n.interfaces[name]; ok {
			iface.Update(stat)
		}
	}
}

func NewNetworkBox() *NetworkBox {
	box := NetworkBox{
		interfaces: map[string]*InterfaceBox{},
	}
	iface := NewInterfaceBox()
	box.selected = iface.networkName
	box.interfaces[iface.networkName] = iface

	return &box
}

func NewInterfaceBox() *InterfaceBox {
	initGraphBox := NewNetworkGraphBox()
	initInfoBox := NewNetworkInfoBox()

	box := randomizeInterfaceData()

	box.info = initInfoBox
	box.graph = initGraphBox
	return &box
}

func (n *NetworkBox) SetSize(w, h int) {
	n.height = h
	n.width = w
}

func (i *InterfaceBox) SetSize(w, h int) {
	i.height = h
	i.width = w
}

func (i *InterfaceBox) layoutSubComponents() {
	graphWidth := i.width * 2 / 3
	infoWidth := i.width - graphWidth
	graphHeight := i.height
	infoHeight := i.height / 2

	i.info.SetSize(infoWidth, infoHeight)
	i.graph.SetSize(graphWidth, graphHeight)
}

func (n *NetworkBox) layoutSubComponents() {

	for _, iface := range n.interfaces {
		iface.SetSize(n.width, n.height-2)
		iface.layoutSubComponents()
	}
}

func (n *NetworkBox) View() string {

	n.layoutSubComponents()

	return lipgloss.NewStyle().
		Border(lipgloss.NormalBorder()).
		Width(n.width).
		Height(n.height).
		Render(n.interfaces[n.selected].View())
}

func (i *InterfaceBox) View() string {
	var usageStr string
	usageStr += fmt.Sprintf(" %s \t\t %s \n", i.ip, i.networkName)

	usageStr += lipgloss.JoinHorizontal(lipgloss.Left, i.graph.View(), i.info.View())

	return usageStr
}

func randomizeInterfaceData() InterfaceBox {

	return InterfaceBox{
		networkName: "wlan0",
		ip:          "192.168.0.102",
	}
}

//func (n *InterfaceBox) SetStats(stats map[string]*InterfaceBox)
