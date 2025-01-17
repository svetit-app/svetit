import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DeviceItemGroupComponent } from './device-item-group.component';

describe('DeviceItemGroupComponent', () => {
  let component: DeviceItemGroupComponent;
  let fixture: ComponentFixture<DeviceItemGroupComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
    imports: [DeviceItemGroupComponent]
})
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(DeviceItemGroupComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
