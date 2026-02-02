package entities

import "time"

type EntityState struct {
	EntityID     string     `json:"entity_id"`
	State        string     `json:"state"`
	Attributes   Attributes `json:"attributes"`
	LastChanged  time.Time  `json:"last_changed"`
	LastReported time.Time  `json:"last_reported"`
	LastUpdated  time.Time  `json:"last_updated"`
	Context      Context    `json:"context"`
}

type Attributes struct {
	// Common
	EffectList          *[]string `json:"effect_list,omitempty"`
	SupportedColorModes *[]string `json:"supported_color_modes,omitempty"`
	FriendlyName        *string   `json:"friendly_name,omitempty"`
	SupportedFeatures   *int      `json:"supported_features,omitempty"`

	// Light-specific (all optional)
	MinColorTempKelvin *int `json:"min_color_temp_kelvin,omitempty"`
	MaxColorTempKelvin *int `json:"max_color_temp_kelvin,omitempty"`
	MinMireds          *int `json:"min_mireds,omitempty"`
	MaxMireds          *int `json:"max_mireds,omitempty"`

	Effect          *string `json:"effect,omitempty"`
	ColorMode       *string `json:"color_mode,omitempty"`
	Brightness      *int    `json:"brightness,omitempty"`
	ColorTempKelvin *int    `json:"color_temp_kelvin,omitempty"`
	ColorTemp       *int    `json:"color_temp,omitempty"`

	HSColor  *[]float64 `json:"hs_color,omitempty"`
	RGBColor *[]int     `json:"rgb_color,omitempty"`
	XYColor  *[]float64 `json:"xy_color,omitempty"`

	Mode     *string `json:"mode,omitempty"`
	Dynamics *string `json:"dynamics,omitempty"`

	// Catch-all for future / unknown attributes
	Extra map[string]any `json:"-"`
}

type Context struct {
	ID       string  `json:"id"`
	ParentID *string `json:"parent_id"`
	UserID   *string `json:"user_id"`
}
