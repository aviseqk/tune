package components

import (
	"fmt"
	"math/rand"
	"strconv"

	"github.com/charmbracelet/bubbles/progress"
	"github.com/charmbracelet/lipgloss"
)

const MAX_DISKS_DISPLAYED = 3

type DiskBox struct {
	width        int
	height       int
	name         string
	used         int
	free         int
	total        int
	progressUsed progress.Model
	progressFree progress.Model
}

type DisksBox struct {
	width  int
	height int
	disks  [MAX_DISKS_DISPLAYED]*DiskBox
}

func (d *DisksBox) SetSize(w, h int) {
	d.width = w
	d.height = h
}

func (d *DiskBox) SetSize(w, h int) {
	d.width = w
	d.height = h
}

func NewDiskBox() *DiskBox {
	box := DiskBox{}
	box.progressFree = progress.New(progress.WithDefaultGradient())
	box.progressUsed = progress.New(progress.WithDefaultGradient())

	box.progressFree.ShowPercentage = false
	box.progressUsed.ShowPercentage = false

	return &box
}

func NewDisksBox() *DisksBox {
	var box DisksBox
	for i := range MAX_DISKS_DISPLAYED {
		subBox := NewDiskBox()
		box.disks[i] = subBox
	}

	return &box
}

func (d *DisksBox) layoutSubComponents() {
	for i := range MAX_DISKS_DISPLAYED {
		subHeight := d.height / 3
		subWidth := d.width
		d.disks[i].SetSize(subWidth, subHeight)
	}
}

func (d *DisksBox) SetDetails(names [MAX_DISKS_DISPLAYED]string, used, total, free [MAX_DISKS_DISPLAYED]int) {
	for i := range MAX_DISKS_DISPLAYED {
		d.disks[i].name = names[i]
		d.disks[i].used = used[i]
		d.disks[i].free = free[i]
		d.disks[i].total = total[i]
	}
}

func (d *DiskBox) SetDetails(name string, used, free, total int) {
	d.name = name
	d.used = used
	d.free = free
	d.total = total
}

func (d *DiskBox) View() string {

	d.name = "Disk" + strconv.Itoa(rand.Intn(17))
	d.free = rand.Intn(1000)
	d.used = rand.Intn(1000)
	d.total = d.free + d.used

	d.progressUsed.Width = d.width - 8
	d.progressFree.Width = d.width - 8

	var percent float64 = float64(d.used) / float64(d.total)
	percentage := int(percent * 100)
	usageStr := fmt.Sprintf("%s  Total: %d GiB\n", d.name, d.total)
	usageStr = usageStr + fmt.Sprintf("Used: %d GiB  Free: %d GiB\n", d.used, d.free)
	usageStr = usageStr + d.progressFree.ViewAs(percent) + strconv.Itoa(percentage) + "%\n"
	usageStr = usageStr + d.progressUsed.ViewAs(percent) + strconv.Itoa(int(float64(d.free)/float64(d.total)*100)) + "%"

	boxStyle := lipgloss.NewStyle().Width(d.width).
		Height(d.height).
		Border(lipgloss.NormalBorder()).
		Align(lipgloss.Center).
		Padding(0, 0)

	return boxStyle.Render(usageStr)
}

func (d *DisksBox) View() string {
	d.layoutSubComponents()

	var viewStr []string

	for i := range MAX_DISKS_DISPLAYED {
		viewStr = append(viewStr, d.disks[i].View())
	}

	return lipgloss.JoinVertical(lipgloss.Left, viewStr...)
}
